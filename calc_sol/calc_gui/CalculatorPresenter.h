#pragma once


#include "ICalculatorView.h"
#pragma managed(push, off)
#include "..\calc_parser\Parser.h"
#include "..\calc_parser\Calculator.h"
#include <sstream>
#include <iostream>
#include <cmath>
#include <fstream>
#pragma managed(pop)



using namespace System;


namespace calc_gui {
	

	/* Model-View-Presenter: presenter class
	Logic of the view*/
	ref class CalculatorPresenter {
	private:
		/* View's interface */
		ICalculatorView^ view;
		/* current calculator - result of compilation */
		calc::Calculator* calculator;
		/*lookup table for functions*/
		calc::StdFunctionLookupTable* flt;
		/*lookup table for constants*/
		calc::StdConstantLookupTable* clt;
		/***** Model ****/
		/*current text of the function*/
		String^ functionText;
		/*function interval: beginning */
		double intervalX1;
		/*function interval: end */
		double intervalX2;
		/*number of points on the X axis */
		int noOfPoints;
		/* minimum calculated value*/
		double minY;
		/* maximum calculated value*/
		double maxY;
		/*current file*/
		String^ currentFileName;
		/*result of calculation: series of points*/
		array<System::Drawing::PointF>^ points;
		/***** End of Model ****/

		void handleStdError(std::exception& e);
	public:

		CalculatorPresenter(ICalculatorView^ view);

		~CalculatorPresenter();

		/* update function's text and parse it; 
		after parsing, create the calulator*/
		void updateFunction(String^ functionText);

		/* calculate a single value and update the view*/
		void calculateValue(double x);

		/* calculate points and update view */
		void drawFunction(double x1, double x2, int noOfPoints);

		/* calculate scale on the X axis (in correlation to noOfPoints)*/
		double getScaleX();

		/* calculate scale on the Y axis (in correlation to resolution)*/
		double getScaleY(int resolutionY);

		array<System::Drawing::PointF>^ getModelCalculatedPoints() {
			return points;
		}

		/* save the function to the file */
		bool saveFile(String^ fileName);

		/* save the function to the current file */
		bool saveFile();

		/*load from file*/
		bool openFile(String^ fileName);

		/* check if save is required*/
		bool isDirty(String^ functionText) {
			return this->functionText == nullptr 
				|| this->functionText != functionText;
		}

		double getMaxY() {
			return maxY;
		}

		double getMinY() {
			return minY;
		}

		String^ getFileName() {
			return currentFileName;
		}
	};
}

