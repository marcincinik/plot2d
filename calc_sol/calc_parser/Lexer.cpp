#include "stdafx.h"
#include "Lexer.h"
#include <istream>
#include <exception>
#include <typeinfo> 
#include <sstream>

using namespace std;

namespace parser {

	/*** Lexer * *** *** *** *** *** *** *** *** *** *** ***/

	auto_ptr<Lexem> Lexer::next(istream& inputStream) {
		auto_ptr<Lexem> lexem;
		if (eof) {
			return auto_ptr<Lexem>();
		}
		do {
			//current state doesn't accept the input
			if (!currentState->update(input)) {

				//state will change
				//clean up the current state
				if (currentState != NULL) {
					try {
						lexem = currentState->getLexem();
					} catch (UnknownTokenException& e) {

						throw UnknownTokenException(
							lineCounter, charCounter, string(e.what()));
					}
					currentState->reset();

				}
				//walk through all possible states of this lexer
				//and check which one is accepting the input
				bool ok = false;
				for (auto it = allStates.begin(); it != allStates.end(); ++it) {
					if ((*it)->update(input)) {
						changeState(*it);
						ok = true;
						break;
					}
				}
				//there is no state in which lexer can process input
				if (!ok) {
					throw UnknownTokenException(lineCounter, charCounter);
				}

			}
			//reading not finished yet - move forward by 1 character
			if (lexem.get() == NULL) {
				readInputChar(inputStream);
				//no more characters in the stream - return token
				if (eof) {
					lexem = currentState->getLexem();
					/*					if (!currentState->isFinal() && lexem.get() == NULL) {
					throw UnknownTokenException(lineCounter, charCounter);
					}*/
				}
			}
		} while (lexem.get() == NULL && !eof);

		//parsing of the current token is finished- clean up
		currentState->reset();
		currentState = startState;
		return lexem;
	}

	Lexer::Lexer() {
		eof = false;
		//magic 0 = epsilon
		input = 0;

		charCounter = 0;
		lineCounter = 1;

		startState = new LexerStateStart();
		allStates.push_back(startState);

		floatState = new LexerStateFloat();
		allStates.push_back(floatState);

		identifierState = new LexerStateIdentifier();
		allStates.push_back(identifierState);

		plusState = new LexerStatePlus();
		allStates.push_back(plusState);

		minusState = new LexerStateMinus();
		allStates.push_back(minusState);

		mulState = new LexerStateMul();
		allStates.push_back(mulState);

		divState = new LexerStateDiv();
		allStates.push_back(divState);

		dashState = new LexerStateDash();
		allStates.push_back(dashState);

		cParenState = new LexerStateCParen();
		allStates.push_back(cParenState);

		oParenState = new LexerStateOParen();
		allStates.push_back(oParenState);

		tildeState = new LexerStateTilde();
		allStates.push_back(tildeState);

		currentState = startState;
	}

	Lexer::~Lexer() {
		delete startState;
		delete plusState;
		delete minusState;
	}

	LexerState* Lexer::changeState(LexerState* state) {
		LexerState* previousState = this->currentState;
		this->currentState = state;
		return previousState;
	}

	void Lexer::readInputChar(istream& inputStream) {

		int c = inputStream.get();
		if (inputStream.eof()) {
			eof = true;
			input = 0;
		} else {
			input = c;
			charCounter++;
			if (input == '\n') {
				charCounter = 0;
				lineCounter++;
			}
		}
	}

	/*** End of Lexer ** *** *** *** *** *** *** *** *** ***/


	/*** LexerStateStart *** *** *** *** *** *** *** *** ***/
	void LexerStateStart::reset() {
		; //do nothing
	}

	bool LexerStateStart::update(char c) {
		bool result;
		//c == 0 - epsilon (not '0')
		//and white spaces
		if (c == 0 || c == ' ' || c == '\t' || c == '\n' || c == '\r') {
			//ignore all whitespaces
			result = true;
		} else {
			result = false;
		}
		return result;
	}


	bool LexerStateStart::isFinal() {
		return true;
	}


	auto_ptr<Lexem> LexerStateStart::getLexem() {
		return auto_ptr<Lexem>();
	}


	/*** End of LexerStateStart  *** *** *** *** *** *** ***/

	/*** LexerStateFloat *** *** *** *** *** *** *** *** ***/
	LexerStateFloat::LexerStateFloat() {
		s = 0;
	}

	void LexerStateFloat::reset() {
		s = 0;
		seen = stringstream();
	}

	bool LexerStateFloat::update(char c) {
		bool accepted;

		switch (s) {
		case 0:
			//begin
			if (c >= '0' && c <= '9') {
				seen.put(c);
				s = 1;
				accepted = true;
			} else {
				accepted = false;
			}
			break;
		case 1:
			//before dot
			if (c >= '0' && c <= '9') {
				seen.put(c);
				accepted = true;
			} else if (c == '.') {
				s = 2;
				seen.put(c);
				accepted = true;
			} else {
				accepted = false;
			}
			break;
		case 2:
			//dot
			if (c >= '0' && c <= '9') {
				seen.put(c);
				s = 3;
				accepted = true;
			} else {
				accepted = false;
			}
			break;
		case 3:
			//after dot
			if (c >= '0' && c <= '9') {
				seen.put(c);
				s = 3;
				accepted = true;
			} else {
				accepted = false;
			}
			break;
		default:
			throw exception("illegal state");

		}
		return accepted;

	}


	bool LexerStateFloat::isFinal() {
		return s == 1 || s == 3;
	}


	auto_ptr<Lexem> LexerStateFloat::getLexem() {
		if (s == 0) {
			//not float
			return auto_ptr<Lexem>();
		} else if (s == 2) {
			//token ends with dot '.'
			throw UnknownTokenException(string("float cannot end with dot '.'"));

		} else {
			//s == 1 || s == 3
			double val;
			//convert string to double
			seen >> val;
			return auto_ptr<Lexem>(new FloatLexem(val));
		}
	}

	/*** End of LexerStateFloat  *** *** *** *** *** *** ***/


	/*** LexerStateIdentifier *** *** *** *** *** *** *** *** ***/
	LexerStateIdentifier::LexerStateIdentifier() {
		s = 0;
	}

	void LexerStateIdentifier::reset() {
		s = 0;
		seen = stringstream();
	}

	bool LexerStateIdentifier::update(char c) {
		bool accepted = false;
		switch (s) {
		case 0:
			//begin
			if (c >= 'a' && c <= 'z'
				|| c >= 'A' && c <= 'Z'
				|| c == '_') {
					seen.put(c);
					s = 1;
					accepted = true;
			} else {
				accepted = false;
			}
			break;
		case 1:
			//first letter or _
			if (c >= 'a' && c <= 'z'
				|| c >= 'A' && c <= 'Z'
				|| c >= '0' && c <= '9'
				|| c == '_') {
					seen.put(c);
					s = 2;
					accepted = true;
			} else {
				accepted = false;
			}
			break;
		case 2:
			//first letter or _ or digit
			if (c >= 'a' && c <= 'z'
				|| c >= 'A' && c <= 'Z'
				|| c >= '0' && c <= '9'
				|| c == '_') {
					seen.put(c);
					s = 2;
					accepted = true;
			} else {
				accepted = false;
			}
			break;
		default:
			throw exception("illegal state");
		}
		return accepted;

	}


	bool LexerStateIdentifier::isFinal() {
		return s == 1 || s == 2;
	}


	auto_ptr<Lexem> LexerStateIdentifier::getLexem() {
		if (s == 0) {
			//not an identifier
			return auto_ptr<Lexem>();
		} else {
			return auto_ptr<Lexem>(new IdentifierLexem(seen.str()));
		}
	}


	/*** End of LexerStateIdentifier  *** *** *** *** *** *** ***/

	/*** LexerStateSingleChar ** *** *** *** *** *** *** ***/
	bool LexerStateSingleChar::isFinalState() {
		return s == 1;
	}

	void LexerStateSingleChar::reset() {
		s = 0;
	}

	bool LexerStateSingleChar::update(char c) {
		bool accepted;
		switch (s) {
		case 0:
			if (c == expectedChar) {
				s = 1;
				accepted = true;
			} else {
				accepted = false;
			}
			break;
		case 1:
			accepted = false;
			break;
		default:
			throw exception("illegal state");
		}
		return accepted;
	}

	bool LexerStateSingleChar::isFinal() {
		return s == 1;
	}


	/*** End of LexerStateSingleChar *** *** *** *** *** ***/

	/*** Lexem *** *** *** *** *** *** *** *** *** *** ***/
	void MinusLexem::accept(LexemVisitor& visitor) {
		visitor.visit(*this);
	}

	void PlusLexem::accept(LexemVisitor& visitor) {
		visitor.visit(*this);
	}

	void MulLexem::accept(LexemVisitor& visitor) {
		visitor.visit(*this);
	}

	void DivLexem::accept(LexemVisitor& visitor) {
		visitor.visit(*this);
	}

	void CParenLexem::accept(LexemVisitor& visitor) {
		visitor.visit(*this);
	}

	void OParenLexem::accept(LexemVisitor& visitor) {
		visitor.visit(*this);
	}

	void TildeLexem::accept(LexemVisitor& visitor) {
		visitor.visit(*this);
	}

	void FloatLexem::accept(LexemVisitor& visitor) {
		visitor.visit(*this);
	}

	void IdentifierLexem::accept(LexemVisitor& visitor) {
		visitor.visit(*this);
	}
	void DashLexem::accept(LexemVisitor& visitor) {
		visitor.visit(*this);
	}

	/*** End of Lexem *** *** *** *** *** *** *** *** ***/

	/*** Exceptions      *** *** *** *** *** *** *** *** ***/
	UnknownTokenException::UnknownTokenException(int lineNo, int charNo) 
		: lineNo(lineNo), charNo(charNo) {
			stringstream sb;
			sb << "Unknown token " << lineNo << ":" << charNo;
			s = sb.str();


	}

	UnknownTokenException::UnknownTokenException(string s) 
		:lineNo(-1), charNo(-1), s(s) {

	}

	UnknownTokenException::UnknownTokenException(int lineNo, int charNo, std::string text) 
		: lineNo(lineNo), charNo(charNo) {
			stringstream sb;
			sb << "Unknown token " << lineNo << ":" << charNo
				<< " " << text;
			s = sb.str();
	}

	const char *UnknownTokenException::what() const {
		return s.c_str();
	}



}