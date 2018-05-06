#ifndef LEXER_H
#define LEXER_H

#include <memory>
#include <istream>
#include <string>
#include <exception>
#include <vector>
#include <sstream>

namespace parser {

	/* forward declaration */
	class Lexem;

	/* forward declaration */
	class LexerState;

	/* forward declaration */
	class LexerStateStart;

	/* forward declaration */
	class LexerStateFloat;

	/* forward declaration */
	class LexerStateIdentifier;

	/* forward declaration */
	class LexerStatePlus;

	/* forward declaration */
	class LexerStateMinus;

	/* forward declaration */
	class LexerStateMul;

	/* forward declaration */
	class LexerStateDiv;

	/* forward declaration */
	class LexerStateDash;

	/* forward declaration */
	class LexerStateCParen;

	/* forward declaration */
	class LexerStateOParen;

	/* forward declaration */
	class LexerStateTilde;

	/* Exception class used by the lexer
	to report leximization problems*/
	class UnknownTokenException : public std::exception {
	private:
		int lineNo;
		int charNo;
		std::string s;
	public:
		UnknownTokenException(int lineNo, int charNo);
		UnknownTokenException(int lineNo, int charNo, std::string text);
		UnknownTokenException(std::string s);
		virtual const char *what() const;
	};

	/* Lexer: process input stream (series of characters)
	and divide it into a series of identified tokens 

	OParen           ::= (
	CParen           ::= )
	Div              ::= /
	Mul              ::= *
	Minus            ::= -
	Plus             ::= +
	Dash             ::= ^
	Tilde			 ::= ~
	Float            ::= [0-9]+(\.[0-9]+)
	Identifier       ::= [A-Za-z_][A-Za-z_0-9]+

	Whitespaces between tokens are all ignored
	*/
	class Lexer {
	private:
		/* Current state of the lexer
		Each state is represented as a separate class
		See the GoF State design pattern

		This field is assigned one of pre-built
		existing states which have the same
		lifecycle as the class*/
		LexerState* currentState;

		/* initial state and a state to accept all white spaces */
		LexerStateStart* startState;

		/* parser 'Float' */
		LexerStateFloat* floatState;

		/* parse 'Identifier' */
		LexerStateIdentifier* identifierState;

		/* parse '+' */
		LexerStatePlus* plusState;

		/* parse '-' */
		LexerStateMinus* minusState;

		/* parse '*' */
		LexerStateMul* mulState;

		/* parse '/' */
		LexerStateDiv* divState;

		/* parse '^' */
		LexerStateDash* dashState;

		/* parse ( */
		LexerStateCParen* cParenState;

		/* parse ) */
		LexerStateOParen* oParenState;

		/* parse ~ */
		LexerStateTilde* tildeState;

		/* all possible states of the lexer */
		std::vector<LexerState*> allStates;

		/* End of stream occured*/
		bool eof;

		/* current input; value = 0 (not '0') is epsilon*/
		char input;

		/* count characters in current line */
		int charCounter;

		/* count lines */
		int lineCounter;

		/* Update current state of the lexer to a new state
		Returns: previous state or null */
		LexerState* changeState(LexerState* state);

		/* Mover forward in the stream of characters by 1 character*/
		void readInputChar(std::istream& inputStream);
	public: 
		Lexer();
		~Lexer();

		/* Read next lexem from the stream.
		Returns: non-null auto_ptr to a Lexem represenitng symbol read from the stream.
		The method returns NULL auto_ptr at the end of file*/
		virtual std::auto_ptr<Lexem> next(std::istream& inputStream);

		/* verify end of stream */
		bool isEof() {
			return eof;
		}

		int getCharNo() {
			return charCounter;
		}

		int getLineNo() {
			return lineCounter;
		}
	};

	/* Forward declaration */
	class LexemVisitor;

	/* Single lexem (token) from grammar - single result of scanning */
	class Lexem {
	private:
	public:
		virtual bool operator ==(const Lexem& other) = 0;

		virtual std::string toString() = 0;

		virtual void accept(LexemVisitor& visitor) = 0;

	};

	/* Minus ::= - */
	class MinusLexem : public Lexem {
	public:
		virtual bool operator ==(const Lexem& other) {
			return typeid(other) == typeid(*this);
		}

		virtual std::string toString() {
			return std::string("-");
		}

		virtual void accept(LexemVisitor& visitor);

	};

	/* Plus ::= + */
	class PlusLexem : public Lexem {
	public:
		virtual bool operator ==(const Lexem& other) {
			return typeid(other) == typeid(*this);
		}

		virtual std::string toString() {
			return std::string("+");
		}
		virtual void accept(LexemVisitor& visitor);
	};

	/* Mul ::= / */
	class MulLexem : public Lexem {
	public:
		virtual bool operator ==(const Lexem& other) {
			return typeid(other) == typeid(*this);
		}

		virtual std::string toString() {
			return std::string("*");
		}
		virtual void accept(LexemVisitor& visitor);
	};

	/* Div ::= / */
	class DivLexem : public Lexem {
	public:
		virtual bool operator ==(const Lexem& other) {
			return typeid(other) == typeid(*this);
		}

		virtual std::string toString() {
			return std::string("/");
		}

		virtual void accept(LexemVisitor& visitor);
	};

	/* Dash ::= ^ */
	class DashLexem : public Lexem {
	public:
		virtual bool operator ==(const Lexem& other) {
			return typeid(other) == typeid(*this);
		}

		virtual std::string toString() {
			return std::string("^");
		}

		virtual void accept(LexemVisitor& visitor);
	};

	/* OParen ::= ( */
	class OParenLexem : public Lexem {
	public:
		virtual bool operator ==(const Lexem& other) {
			return typeid(other) == typeid(*this);
		}

		virtual std::string toString() {
			return std::string("(");
		}

		virtual void accept(LexemVisitor& visitor);
	};

	/* CParen ::= ) */
	class CParenLexem : public Lexem {
	public:
		virtual bool operator ==(const Lexem& other) {
			return typeid(other) == typeid(*this);
		}

		virtual std::string toString() {
			return std::string(")");
		}

		virtual void accept(LexemVisitor& visitor);
	};

	/* Tilde ::= ~ */
	class TildeLexem : public Lexem {
	public:
		virtual bool operator ==(const Lexem& other) {
			return typeid(other) == typeid(*this);
		}

		virtual std::string toString() {
			return std::string("~");
		}

		virtual void accept(LexemVisitor& visitor);
	};

	/* Float ::= [0-9]+(\.[0-9]+) */
	class FloatLexem : public Lexem {
	private:
		double d;
	public:
		FloatLexem(double d) : d(d) {}

		virtual bool operator ==(const Lexem& other) {
			const FloatLexem* other2 = dynamic_cast<const FloatLexem*>(&other);
			return other2 != NULL && other2->d == this->d;
		}

		virtual std::string toString() {
			std::stringstream s;
			s << d;
			return s.str();
		}

		double getValue() {
			return d;
		}

		virtual void accept(LexemVisitor& visitor);
	};

	/* Identifier       ::= [A-Za-z_][A-Za-z_0-9]+ */
	class IdentifierLexem : public Lexem {
	private:
		std::string s;
	public:
		IdentifierLexem(std::string s) : s(s) {}

		virtual bool operator ==(const Lexem& other) {			
			const IdentifierLexem* other2 = dynamic_cast<const IdentifierLexem*>(&other);
			return other2 != NULL && other2->s == this->s;
		}

		virtual std::string toString() {
			return s;
		}

		std::string getIdentifier() {
			return s;
		}

		virtual void accept(LexemVisitor& visitor);

	};

	/* GoF visitor design pattern.
	Interface for the visitor for Lexems.
	Lexems don't create any composite structures,
	but to enable polymorphic processing of lexems,
	I am implementing the Visitor pattern here*/
	class LexemVisitor {
	public:
		virtual void visit(MinusLexem& minusLexem) = 0;
		virtual void visit(PlusLexem& plusLexem) = 0;
		virtual void visit(MulLexem& mulLexem) = 0;
		virtual void visit(DivLexem& divLexem) = 0;
		virtual void visit(DashLexem& divLexem) = 0;
		virtual void visit(CParenLexem& cParenLexem) = 0;
		virtual void visit(OParenLexem& oParenLexem) = 0;
		virtual void visit(TildeLexem& tildeLexem) = 0;
		virtual void visit(FloatLexem& tildeLexem) = 0;
		virtual void visit(IdentifierLexem& tildeLexem) = 0;
	};


	/* [GoF] State design pattern
	Each subclass represents one of possible states of an Lexer object.	*/
	class LexerState {
	public:
		/* Return parsed lexem. 
		Returns: null if no lexems yet parsed; this method will return non-null only when
		update method returns true*/
		virtual std::auto_ptr<Lexem> getLexem() = 0;

		/* Reset internal state of this object */
		virtual void reset() = 0;

		/* Update state based on the input */
		virtual bool update(char c) = 0;

		/* Returns: true if in final state*/
		virtual bool isFinal() = 0;
	};

	/* The state of lexer when parsing space between token.
	Usually white spaces. This state is also accepting epsilon (beginning)*/
	class LexerStateStart : public LexerState {
	public:
		/* see LexerState */
		virtual std::auto_ptr<Lexem> getLexem();

		/* see LexerState */
		virtual void reset();

		/* see LexerState */
		virtual bool update(char c);

		/* see LexerState */
		virtual bool isFinal();
	};

	/* Parse 'Float' tokens */
	class LexerStateFloat : public LexerState {
	private:
		/* Internal state
		0 - start
		1 - before dot
		2 - '.' dot
		3 - after dot

		transitions:
		0 -> 1 -> 2 -> 3 -> F
		0 -> 1 -> F
		*/
		int s;

		std::stringstream seen;

	public:
		LexerStateFloat();
		/* see LexerState */
		virtual std::auto_ptr<Lexem> getLexem();

		/* see LexerState */
		virtual void reset();

		/* see LexerState */
		virtual bool update(char c);

		/* see LexerState */
		virtual bool isFinal();
	};

	/* Parse 'Identifier' tokens */
	class LexerStateIdentifier : public LexerState {
	private:
		/* Internal state
		0 - start
		1 - first letter or _
		2 - first letter or _ or digit

		transitions:
		0 -> 1 -> F
		0 -> 1 -> 2 -> F
		*/
		int s;

		std::stringstream seen;
	public:
		LexerStateIdentifier();

		/* see LexerState */
		virtual std::auto_ptr<Lexem> getLexem();

		/* see LexerState */
		virtual void reset();

		/* see LexerState */
		virtual bool update(char c);

		/* see LexerState */
		virtual bool isFinal();
	};

	/* The state of lexer when parsing single character token.*/
	class LexerStateSingleChar : public LexerState {
	private:
		/* Transitions
		0 - start
		1 - excpected character (final)

		0->1(f)

		*/
		int s;

		/* character expected */
		char expectedChar;
	protected:
		LexerStateSingleChar(char expectedChar)
			: s(0), expectedChar(expectedChar) {};
		/* Returns: true if current state of the object is final (accepting)*/
		bool isFinalState();
	public:
		/* see LexerState */
		virtual std::auto_ptr<Lexem> getLexem() = 0;

		/* see LexerState */
		virtual void reset();

		/* see LexerState */
		virtual bool update(char c);

		/* see LexerState */
		virtual bool isFinal();
	};

	/* Parse '+' token */
	class LexerStatePlus : public LexerStateSingleChar {
	public:
		LexerStatePlus() : LexerStateSingleChar('+') {}

		/* see LexerState */
		virtual std::auto_ptr<Lexem> getLexem() {
			std::auto_ptr<Lexem> result(new PlusLexem());
			return result;
		}
	};

	/* Parse '-' token */
	class LexerStateMinus : public LexerStateSingleChar {
	public:
		LexerStateMinus() : LexerStateSingleChar('-') {}

		/* see LexerState */
		virtual std::auto_ptr<Lexem> getLexem() {
			std::auto_ptr<Lexem> result(new MinusLexem());
			return result;
		}
	};

	/* Parse '*' token */
	class LexerStateMul : public LexerStateSingleChar {
	public:
		LexerStateMul() : LexerStateSingleChar('*') {}

		/* see LexerState */
		virtual std::auto_ptr<Lexem> getLexem() {
			std::auto_ptr<Lexem> result(new MulLexem());
			return result;
		}
	};

	/* Parse '/' token */
	class LexerStateDiv : public LexerStateSingleChar {
	public:
		LexerStateDiv() : LexerStateSingleChar('/') {}

		/* see LexerState */
		virtual std::auto_ptr<Lexem> getLexem() {
			std::auto_ptr<Lexem> result(new DivLexem());
			return result;
		}
	};

	/* Parse '^' token */
	class LexerStateDash : public LexerStateSingleChar {
	public:
		LexerStateDash() : LexerStateSingleChar('^') {}

		/* see LexerState */
		virtual std::auto_ptr<Lexem> getLexem() {
			std::auto_ptr<Lexem> result(new DashLexem());
			return result;
		}
	};

	/* Parse '(' token */
	class LexerStateOParen : public LexerStateSingleChar {
	public:
		LexerStateOParen() : LexerStateSingleChar('(') {}

		/* see LexerState */
		virtual std::auto_ptr<Lexem> getLexem() {
			std::auto_ptr<Lexem> result(new OParenLexem());
			return result;
		}
	};

	/* Parse ')' token */
	class LexerStateCParen : public LexerStateSingleChar {
	public:
		LexerStateCParen() : LexerStateSingleChar(')') {}

		/* see LexerState */
		virtual std::auto_ptr<Lexem> getLexem() {
			std::auto_ptr<Lexem> result(new CParenLexem());
			return result;
		}
	};

	/* Parse '~' token */
	class LexerStateTilde : public LexerStateSingleChar {
	public:
		LexerStateTilde() : LexerStateSingleChar('~') {}

		/* see LexerState */
		virtual std::auto_ptr<Lexem> getLexem() {
			std::auto_ptr<Lexem> result(new TildeLexem());
			return result;
		}
	};

}

#endif

