#include "stdafx.h"
#include <msclr\marshal_cppstd.h>
#include "CalculatorPresenter.h"
#include "ICalculatorView.h"

using namespace calc;
using namespace parser;
using namespace msclr::interop;

namespace calc_gui {

	CalculatorPresenter::CalculatorPresenter(ICalculatorView^ view) {
		this->view = view;
		calculator = NULL;
		flt = new StdFunctionLookupTable();
		clt = new StdConstantLookupTable();
	}

	CalculatorPresenter::~CalculatorPresenter() {
		if (calculator != NULL) {
			delete calculator;
		}
		if (flt != NULL) {
			delete flt;
		}
		if (clt != NULL) {
			delete clt;
		}
	}

	void CalculatorPresenter::handleStdError(std::exception& e) {
		String^ s = marshal_as<String^>(e.what());
		if (s == nullptr || s->Length == 0) {
			String^ typeName = marshal_as<String^>(typeid(e).name());
			view->showError("unidentified error " + typeName);
		} else {
			view->showError(s);
		}
	}

	void CalculatorPresenter::updateFunction(String^ functionText) {
		//guard condition - if text unmodified, do nothing
		if (calculator == NULL || functionText != this->functionText) {
			this->functionText = functionText;
			std::string s = marshal_as<std::string>(functionText);
			if (s.length() > 0) {
				std::stringstream parserInput;
				parserInput << s;
				Parser parser(parserInput, clt, flt);
				AstNode* expr = NULL;
				try {
					parser.begin();
					expr = parser.expr();
				} catch (std::exception& e) {
					handleStdError(e);
				}
				if (expr != NULL) {
					if (calculator != NULL) {
						delete calculator;
						calculator = NULL;
					}
					try {
						calculator = new Calculator(std::string("x"), flt, clt, expr);
						view->clearError();
					} catch (std::exception& e) {
						handleStdError(e);
					}
					delete expr;
				}
			}
		}
	}

	void CalculatorPresenter::calculateValue(double x) {
		if (calculator != NULL) {
			double y = calculator->calculate(x);
			view->updateFunctionValue(y);
		}
	}

	void CalculatorPresenter::drawFunction(double x1, double x2, int noOfPoints) {
		intervalX1 = x1;
		intervalX2 = x2;
		this->noOfPoints = noOfPoints;
		points = gcnew array<System::Drawing::PointF>(noOfPoints);
		if (calculator != NULL) {
			double deltaX = (x2-x1) / noOfPoints;
			minY = maxY = 0.0;
			for (int i = 0; i < noOfPoints; i++) {
				points[i].X = (float)(x1 + deltaX*i);
				double y = calculator->calculate(points[i].X);
				if (_isnan(y) || !_finite(y)) {
					//not-a-number
					y = 0.0f;
				}
				if (y < minY) {
					minY = y;
				}
				if (y > maxY) {
					maxY = y;
				}
				points[i].Y = (float)y;
			}
		}
		view->updateGraph();
	}

	double CalculatorPresenter::getScaleX() {
		if (intervalX2 - intervalX1 != 0.0) {
			return noOfPoints / (intervalX2 - intervalX1);
		} else {
			return 0.0;
		}
	}

	double CalculatorPresenter::getScaleY(int resolutionY) {
		if (maxY - minY != 0.0) {
			return resolutionY / Math::Abs(maxY - minY);
		} else {
			return 1.0;
		}
	}

	bool CalculatorPresenter::saveFile(String^ fileName) {
		if (functionText != nullptr && calculator != NULL) {
			std::string fileNameStr = marshal_as<std::string>(fileName);
			String^ s = this->functionText;
			std::string functionTxtStr = marshal_as<std::string>(s);
			std::ofstream outputStream;
			outputStream.open(fileNameStr, std::ofstream::out | std::ofstream::trunc);
			//text of the function + newline
			outputStream << functionTxtStr << std::endl;
			//Reverse Polish Notation + newline
			calculator->save(outputStream);
			outputStream << std::endl;
			outputStream.flush();
			outputStream.close();
			currentFileName = fileName;
			return true;
		} else {
			return false;
		}
	}

	bool CalculatorPresenter::saveFile() {
		if (currentFileName != nullptr) {
			return saveFile(currentFileName);
		} else {
			return false;
		}
	}

	bool CalculatorPresenter::openFile(String^ fileName) {
		char lineBuf[4096];
		std::string fileNameStr = marshal_as<std::string>(fileName);
		std::ifstream inputStream;
		inputStream.open(fileNameStr, std::ifstream::in);
		inputStream.getline(lineBuf, 4095);
		if (inputStream.gcount() <= 0) {
			inputStream.close();
			return false;
		}
		String^ functionTxtStr = marshal_as<String^>(lineBuf);

		if (calculator != NULL) {
			delete calculator;
			calculator = NULL;
		}
		calculator = new Calculator(std::string("x"), flt, clt, inputStream);
		view->updateFunctionText(functionTxtStr);
		inputStream.close();
		currentFileName = fileName;
		return true;
	}
}