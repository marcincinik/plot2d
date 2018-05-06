#pragma once

using namespace System;
namespace calc_gui {

	/* Model-View-Presenter: interface of the view */
	public interface class ICalculatorView  {
	public:
		/* update function calculation result for a single value
		in the view*/
		virtual void updateFunctionValue(double v) ;

		/* update function's text */
		virtual void updateFunctionText(String^ functionText);

		/* request to redraw graph; */
		virtual void updateGraph() ;

		virtual void showError(String^ msg);

		virtual void clearError();
	};

}

