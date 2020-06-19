/*
The Nfy library is distributed under the MIT License (MIT)
https://opensource.org/licenses/MIT
See the LICENSE file or Nfy.h for license details.
Copyright (c) 2020 James Boer
*/

#pragma once

#include "../../Include/Nfy.hpp"

#include "catch.hpp"

class INotify
{
public:
	virtual ~INotify() {}
	virtual void NoArg() = 0;
	virtual void SingleArg(int arg) = 0;
	virtual void MultiArg(double arg1, double arg2) = 0;
	virtual bool ReturnVal(bool val) = 0;
};

class Observer : public INotify
{
public:
	virtual void NoArg() override { m_noArg = true; }
	bool NoArgCalled() const { return m_noArg; }

	virtual void SingleArg(int arg) override { m_singleArg = arg; }
	int GetSingleArg() const { return m_singleArg; }

	virtual void MultiArg(double arg1, double arg2) override { m_multiArg1 = arg1; m_multiArg2 = arg2; }
	double GetMultiArg1() const { return m_multiArg1; }
	double GetMultiArg2() const { return m_multiArg2; }

	virtual bool ReturnVal(bool val) override { m_returnVal = val; return m_returnVal; }
	bool GetReturnVal() const { return m_returnVal; }

private:
	bool m_noArg = false;
	int m_singleArg = 0;
	double m_multiArg1 = 0.0;
	double m_multiArg2 = 0.0;
	bool m_returnVal = false;
};
