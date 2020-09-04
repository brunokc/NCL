//
// Obtained from http://stackoverflow.com/questions/12403920/using-stdfunction-as-a-delegate-in-c11,
// 3rd answer.
//

#pragma once
#include <functional>
#include <memory>

namespace mezutils
{
    template<typename Method>
    struct Delegated;

    template< typename Return, typename ... Args >
    struct MethodInterface
    {
        virtual ~MethodInterface(){}

        Return operator()( Args ... args )
        {
            return execute( args... );
        }

        virtual Return execute( Args ... args ) = 0;
    };

    template< typename Return, typename ... Args >
    struct SimpleMethod
        : public MethodInterface< Return, Args ... >
    {
        typedef Return (*Method)(Args ...);
        SimpleMethod( Return (*call)(Args ...) )
            : method( call )
        {}

        Return execute( Args ... args )
        {
            return method( args ... );
        }

    private:
        Method method;
    };

    template<typename Owner, typename Return, typename ... Args >
    struct MemberMethod
        : public MethodInterface< Return, Args ... >
    {
        typedef Return (Owner::*Method)(Args ...);
        MemberMethod( Owner * object, Method call )
            : owner( object ), method( call )
        {}

        Return execute( Args ... args )
        {
            return (owner->*method)( args ... );
        }
    private:
        Method method;
        Owner * owner;
    };

    template<typename Owner, typename Return, typename ... Args >
    struct ConstMemberMethod
        : public MethodInterface< Return, Args ... >
    {
        typedef Return (Owner::*Method)(Args ...) const;
        ConstMemberMethod( Owner * object, Method call )
            : owner( object ), method( call )
        {}

        Return execute( Args ... args )
        {
            return (owner->*method)( args ... );
        }
    private:
        Method method;
        Owner * owner;
    };

    template<typename Owner, typename Return, typename ... Args >
    struct VolatileMemberMethod
        : public MethodInterface< Return, Args ... >
    {
        typedef Return (Owner::*Method)(Args ...) volatile;
        VolatileMemberMethod( Owner * object, Method call )
            : owner( object ), method( call )
        {}

        Return execute( Args ... args )
        {
            return (owner->*method)( args ... );
        }
    private:
        Method method;
        Owner * owner;
    };

    template< typename Return, typename ... Args >
    struct MethodExecutor
    {
        typedef MethodInterface< Return, Args ... > Interface;
        typedef std::shared_ptr<Interface> InterfacePtr;

        MethodExecutor() throw ()
        {}

        MethodExecutor( Return (*call)(Args ...) ) throw ()
        {
            executor = InterfacePtr(new SimpleMethod< Return, Args ... >( call ));
        }

        template< typename Owner >
        MethodExecutor( Owner & owner, Return (Owner::*Member)( Args ... ) ) throw ()
        {
            executor = InterfacePtr(new MemberMethod< Owner, Return, Args ... >( &owner,  Member ));
        }

        template< typename Owner >
        MethodExecutor( Owner * owner, Return (Owner::*Member)( Args ... ) ) throw ()
        {
            executor = InterfacePtr(new MemberMethod< Owner, Return, Args ... >( owner,  Member ));
        }

        template< typename Owner >
        MethodExecutor( Owner & owner, Return (Owner::*Member)( Args ... ) const ) throw ()
        {
            executor = InterfacePtr(new ConstMemberMethod< Owner, Return, Args ... >( &owner,  Member ));
        }

        template< typename Owner >
        MethodExecutor( Owner * owner, Return (Owner::*Member)( Args ... ) const ) throw ()
        {
            executor = InterfacePtr(new ConstMemberMethod< Owner, Return, Args ... >( owner,  Member ));
        }

        template< typename Owner >
        MethodExecutor( Owner & owner, Return (Owner::*Member)( Args ... ) volatile ) throw ()
        {
            executor = InterfacePtr(new VolatileMemberMethod< Owner, Return, Args ... >( &owner,  Member ));
        }

        template< typename Owner >
        MethodExecutor( Owner * owner, Return (Owner::*Member)( Args ... ) volatile ) throw ()
        {
            executor = InterfacePtr(new VolatileMemberMethod< Owner, Return, Args ... >( owner,  Member ));
        }

        virtual ~MethodExecutor(){}

        Return operator()( Args ... args )
        {
            return executor->execute( args ... );
        }

        MethodExecutor & operator = ( Return (*Method)(Args...) )
        {
            executor = InterfacePtr(new SimpleMethod< Return, Args ... >( Method ));
            return *this;
        }

    private:
        InterfacePtr executor;
    };

#define MEZ_CLASS_GET_FUNCTION_IMPL( \
    TEMPLATE_LIST, PADDING_LIST, LIST, COMMA, CALL_OPT, X2, X3, X4) \
    template<class _Ret COMMA LIST(_CLASS_TYPE)> \
    struct Delegated<_Ret CALL_OPT (LIST(_TYPE))> \
    { \
        typedef MethodExecutor<_Ret COMMA LIST(_TYPE)> type; \
    };

#define MEZ_CLASS_GET_FUNCTION_IMPL_CALLS( \
    TEMPLATE_LIST, PADDING_LIST, LIST, COMMA, CALL_OPT, X2, X3, X4) \
    _VARIADIC_CALL_OPT_X1(MEZ_CLASS_GET_FUNCTION_IMPL, \
    TEMPLATE_LIST, PADDING_LIST, LIST, COMMA, __cdecl, X2, X3, X4)

    _VARIADIC_EXPAND_0X(MEZ_CLASS_GET_FUNCTION_IMPL_CALLS, , , , )

#undef MEZ_CLASS_GET_FUNCTION_IMPL_CALLS
#undef MEZ_CLASS_GET_FUNCTION_IMPL

    template< typename Function >
    class Delegate
        : public Delegated< Function >::type
    {
        typedef typename Delegated< Function >::type Base;
    public:
        Delegate()
        {
        }

        Delegate( Function call )
            : Base( call )
        {
        }

        template< typename Return, typename ... Args >
        Delegate( Return (*call)(Args ...) )
            : Base( call )
        {
        }

        template< typename Owner, typename Return, typename ... Args >
        Delegate( Owner & owner, Return (Owner::*Member)( Args ... ) ) throw ()
            : Base( owner, Member )
        {
        }

        template< typename Owner, typename Return, typename ... Args >
        Delegate( Owner * owner, Return (Owner::*Member)( Args ... ) ) throw ()
            : Base( owner, Member )
        {
        }

        template< typename Owner, typename Return, typename ... Args >
        Delegate( Owner & owner, Return (Owner::*Member)( Args ... ) const ) throw ()
            : Base( owner, Member )
        {
        }

        template< typename Owner, typename Return, typename ... Args >
        Delegate( Owner * owner, Return (Owner::*Member)( Args ... ) const ) throw ()
            : Base( owner, Member )
        {
        }

        template< typename Owner, typename Return, typename ... Args >
        Delegate( Owner & owner, Return (Owner::*Member)( Args ... ) volatile ) throw ()
            : Base( owner, Member )
        {
        }

        template< typename Owner, typename Return, typename ... Args >
        Delegate( Owner * owner, Return (Owner::*Member)( Args ... ) volatile ) throw ()
            : Base( owner, Member )
        {
        }

        ~Delegate(){}

        template< typename Return, typename ... Args >
        Delegate & operator = ( Return (*Method)(Args...) )
        {
            Base::operator = ( Method );
            return *this;
        }

        Delegate & operator = ( Function call )
        {
            Base::operator = ( call );
            return *this;
        }
    };
}
