# Nfy ![CI](https://github.com/JamesBoer/Nfy/workflows/CI/badge.svg)
Nfy is a template-based notification library designed to simplify the implementation of an interface-based observer pattern in C++.  

## What is Nfy Used For?
The Nfy library allows you to send messages from one class to another through the use of an interface.  Observer classes are derived from this interface, register themselves with the notification system, and can then receive messages.  The notification class retains a list of weak pointers to these observers, allowing a the notification system to send messages to an arbitrary number of observers.

## Building Nfy
The Nfy library is distributed as a single header file, Nfy.hpp, so you can include the library header and start using it immediately.  Nfy requires a C++ 14 compliant compiler.

## How to Use Nfy
The Nfy library consists of a single class, called ```Notify```, which is in the namespace Nfy.  For brevity and clarity, all subsequent examples are assuming ```using namespace Nfy``` has been added earlier in the source file.  Let's examine how we might use this class.  

### Simple Events
First, let's presume we want to create an interface from which we can derive other classes, so as to receive notification events.  Let's call this class ```INotify```, and assume we have a single notification event we want to track, for which we'll create a handler function called ```void OnEventX(int arg)```.  This function does not return any value, and takes a single integer argument.

``` c++
class INotify
{
public:
    virtual ~INotify() {}
    virtual void OnEventX(int arg) = 0;
};
```    
    
Now let's derive a concrete implementation from this interface, which we'll call ```Observer```.  

``` c++
class Observer : public INotify
{
public:
    virtual void OnEventX(int arg) override 
    {
        // EventX called!
    }
};
```   
All instances of this ```Observer``` class, once registered with an approprate notifier object, will have it's OnEventX() member function called when involved from the notifier object.  Let's examine how this is done.

``` c++
// Create notify bus object
Notifier<INotify> notifier;

// Create observer object and register with notify bus
auto observer = std::make_shared<Observer>();
notifier.Register(observer);

// Call OnEventX() with parameter for all registered observers
notifier.Notify(&INotify::OnEventX, 42);
``` 

We see here a single observer registered with a single notify bus.  The ```OnEventX``` function call is invoked on all registered observers with the ```Notify``` call, where we passed both the function signature and any subsequent parameters.

You'll note that we've created the observer object with a ```std::shared_ptr```.  This is necessary for the ```Notifier``` class to hold onto the observers as weak pointers.  Using weak pointers means you don't have to remember to explicitly unregister the observer.  ```Notifier``` will check internal pointers as it iterates through them, and erase any weak pointers that have expired.

### Events with Boolean Return Values

Notifications using function signatures with a Boolean return value have a special meaning for Nfy.  The ```Notifier``` class will call all registered observers until a function returns a true value, which signifies "I've handled this event, so you can stop processing the requests now."  There may be cases where it only makes sense for one observer to handle an event, after which all other observers should not be notified.

Let's assume our ```INotify``` interface looked like this, with a Boolean return value on the ```OnEventX()``` function:

``` c++
class INotify
{
public:
    virtual ~INotify() {}
    virtual bool OnEventX(int arg) = 0;
};
```   

We can now check to see if at least one observer returned true, signaling that the event was "handled". 

``` c++
// Call OnEventX() with parameter for all registered observers
if (notifier.Notify(&INotify::OnEventX, 42))
    // Event has been handled
``` 

### Notifier Options

By default, all operations in the ```Notifier``` class are thread safe, protected by a ```std::mutex```, and the internal ```std::vector``` uses the default allocator.  Both of these defaults can be overridden.

To disable thread safety in situations where you wish to make this performance-safety trade-off, you can create a single-threaded ```Notifier``` bus object which uses a custom allocator as follows:

``` c++
// Create notify bus object
Notifier<INotify, SingleThreaded, MyAllocator<std::weak_ptr<INotify>>> notifier;

``` 