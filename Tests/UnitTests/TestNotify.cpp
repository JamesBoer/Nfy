/*
The Nfy library is distributed under the MIT License (MIT)
https://opensource.org/licenses/MIT
See the LICENSE file or Nfy.hpp for license details.
Copyright (c) 2020 James Boer
*/

#include "UnitTest.h"

using namespace Nfy;

TEST_CASE("Test Notify", "[Notify]")
{

    SECTION("Single no-argument notification")
    {
		// Create notify bus
		Notifier<INotify, SingleThreaded> notify;

		// Create notified object and register with notify bus
		auto observer = std::make_shared<Observer>();
		notify.Register(observer);

		// Notify the object
		notify.Notify(&INotify::NoArg);

		// Ensure the object's function has been called
		REQUIRE(observer->NoArgCalled());
	}

	SECTION("Multiple no-argument notification")
	{
		// Create notify bus
		Notifier<INotify, SingleThreaded> notifier;

		// Create notified object and register with notify bus
		auto observer1 = std::make_shared<Observer>();
		auto observer2 = std::make_shared<Observer>();
		auto observer3 = std::make_shared<Observer>();
		notifier.Register(observer1);
		notifier.Register(observer2);
		notifier.Register(observer3);

		// Notify the object
		notifier.Notify(&INotify::NoArg);

		// Ensure the object's function has been called
		REQUIRE(observer1->NoArgCalled() == true);
		REQUIRE(observer2->NoArgCalled() == true);
		REQUIRE(observer3->NoArgCalled() == true);
	}

	SECTION("Single no-argument notification unregister")
	{
		// Create notify bus
		Notifier<INotify, SingleThreaded> notifier;

		// Create notified object and register with notify bus
		auto observer = std::make_shared<Observer>();
		notifier.Register(observer);
		notifier.Unregister(observer);

		// Notify the object
		notifier.Notify(&INotify::NoArg);

		// Ensure the object's function has been called
		REQUIRE(observer->NoArgCalled() == false);
	}


	SECTION("Multiple no-argument notification unregister")
	{
		// Create notify bus
		Notifier<INotify, SingleThreaded> notifier;

		// Create notified object and register with notify bus
		auto observer1 = std::make_shared<Observer>();
		auto observer2 = std::make_shared<Observer>();
		auto observer3 = std::make_shared<Observer>();
		notifier.Register(observer1);
		notifier.Register(observer2);
		notifier.Register(observer3);

		// Unregister just one observer
		notifier.Unregister(observer2);

		// Notify the object
		notifier.Notify(&INotify::NoArg);

		// Ensure the object's function have been called or not called as appropriate
		REQUIRE(observer1->NoArgCalled() == true);
		REQUIRE(observer2->NoArgCalled() == false);
		REQUIRE(observer3->NoArgCalled() == true);
	}

	SECTION("Single single-argument notification")
	{
		// Create notify bus
		Notifier<INotify, SingleThreaded> notifier;

		// Create notified object and register with notify bus
		auto observer = std::make_shared<Observer>();
		notifier.Register(observer);

		// Notify the object
		notifier.Notify(&INotify::SingleArg, 34);

		// Ensure the object's function has been called
		REQUIRE(observer->GetSingleArg() == 34);
	}

	SECTION("Single multi-argument notification")
	{
		// Create notify bus
		Notifier<INotify, SingleThreaded> notifier;

		// Create notified object and register with notify bus
		auto observer = std::make_shared<Observer>();
		notifier.Register(observer);

		// Notify the object
		notifier.Notify(&INotify::MultiArg, 12.34, 56.78);

		// Ensure the object's function has been called
		REQUIRE(observer->GetMultiArg1() == 12.34);
		REQUIRE(observer->GetMultiArg2() == 56.78);
	}

	SECTION("Single return value notification")
	{
		// Create notify bus
		Notifier<INotify, SingleThreaded> notifier;

		// Create notified object and register with notify bus
		auto observer = std::make_shared<Observer>();
		notifier.Register(observer);

		// Notify the object
		REQUIRE(notifier.Notify(&INotify::ReturnVal, true) == true);

		// Ensure the object's function has been called
		REQUIRE(observer->GetReturnVal() == true);
	}

	SECTION("Single return false notification")
	{
		// Create notify bus
		Notifier<INotify, SingleThreaded> notifier;

		// Create notified object and register with notify bus
		auto observer = std::make_shared<Observer>();
		notifier.Register(observer);

		// Notify the object
		REQUIRE(notifier.Notify(&INotify::ReturnVal, false) == false);

		// Ensure the object's function has been called
		REQUIRE(observer->GetReturnVal() == false);
	}

	SECTION("Multiple return value notification")
	{
		// Create notify bus
		Notifier<INotify, SingleThreaded> notifier;

		// Create notified object and register with notify bus
		auto observer1 = std::make_shared<Observer>();
		auto observer2 = std::make_shared<Observer>();
		auto observer3 = std::make_shared<Observer>();
		notifier.Register(observer1);
		notifier.Register(observer2);
		notifier.Register(observer3);

		// Notify the object
		REQUIRE(notifier.Notify(&INotify::ReturnVal, true) == true);

		// Ensure that only one observer has been notified
		size_t count = 0;
		count += observer1->GetReturnVal() ? 1 : 0;
		count += observer2->GetReturnVal() ? 1 : 0;
		count += observer3->GetReturnVal() ? 1 : 0;
		REQUIRE(count == 1);
	}

}
