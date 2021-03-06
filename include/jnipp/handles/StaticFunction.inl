// Copyright since 2016 : Evgenii Shatunov (github.com/FrankStain/jnipp)
// Apache 2.0 License
#pragma once


namespace Jni
{
	template< typename TNativeReturnType, typename... TNativeArguments >
	StaticFunction<TNativeReturnType, TNativeArguments...>::StaticFunction( const StaticFunction& other )
		: m_class_handle{ other.m_class_handle }
		, m_function_id{ other.m_function_id }
	{

	}

	template< typename TNativeReturnType, typename... TNativeArguments >
	StaticFunction<TNativeReturnType, TNativeArguments...>::StaticFunction( StaticFunction&& other )
		: m_class_handle{ std::move( other.m_class_handle ) }
		, m_function_id{ std::move( other.m_function_id ) }
	{

	}

	template< typename TNativeReturnType, typename... TNativeArguments >
	StaticFunction<TNativeReturnType, TNativeArguments...>::StaticFunction( const std::string& class_name, const std::string& function_name )
		: StaticFunction{ class_name.c_str(), function_name.c_str() }
	{

	}

	template< typename TNativeReturnType, typename... TNativeArguments >
	StaticFunction<TNativeReturnType, TNativeArguments...>::StaticFunction( const Class& class_handle, const std::string& function_name )
		: StaticFunction{ class_handle, function_name.c_str() }
	{

	}

	template< typename TNativeReturnType, typename... TNativeArguments >
	StaticFunction<TNativeReturnType, TNativeArguments...>::StaticFunction( const char* class_name, const char* function_name )
		: StaticFunction{ Class{ class_name }, function_name }
	{

	}

	template< typename TNativeReturnType, typename... TNativeArguments >
	StaticFunction<TNativeReturnType, TNativeArguments...>::StaticFunction( const Class& class_handle, const char* function_name )
		: StaticFunction{ class_handle, function_name, IGNORE_FAILURE }
	{
		JNI_ENSURES( m_function_id != 0 );
	}

	template< typename TNativeReturnType, typename... TNativeArguments >
	StaticFunction<TNativeReturnType, TNativeArguments...>::StaticFunction( const std::string& class_name, const std::string& function_name, IgnoreFailure )
		: StaticFunction{ class_name.c_str(), function_name.c_str(), IGNORE_FAILURE }
	{

	}

	template< typename TNativeReturnType, typename... TNativeArguments >
	StaticFunction<TNativeReturnType, TNativeArguments...>::StaticFunction( const Class& class_handle, const std::string& function_name, IgnoreFailure )
		: StaticFunction{ class_handle, function_name.c_str(), IGNORE_FAILURE }
	{

	}

	template< typename TNativeReturnType, typename... TNativeArguments >
	StaticFunction<TNativeReturnType, TNativeArguments...>::StaticFunction( const char* class_name, const char* function_name, IgnoreFailure )
		: StaticFunction{ Class{ class_name }, function_name, IGNORE_FAILURE }
	{

	}

	template< typename TNativeReturnType, typename... TNativeArguments >
	StaticFunction<TNativeReturnType, TNativeArguments...>::StaticFunction( const Class& class_handle, const char* function_name, IgnoreFailure )
		: m_class_handle{ class_handle }
	{
		JNI_EXPECTS( m_class_handle );
		JNI_RETURN_IF_E( !VirtualMachine::IsValid(), , "%s:%d - Attempt to use Uninitialized virtual machine.", __func__, __LINE__ );

		auto local_env	= VirtualMachine::GetLocalEnvironment();
		m_function_id	= local_env->GetStaticMethodID( *m_class_handle, function_name, Signature::GetString() );

		if( local_env->ExceptionCheck() == JNI_TRUE )
		{
			local_env->ExceptionDescribe();
			local_env->ExceptionClear();
			m_function_id = nullptr;
		}
	}

	template< typename TNativeReturnType, typename... TNativeArguments >
	inline TNativeReturnType StaticFunction<TNativeReturnType, TNativeArguments...>::Call( const TNativeArguments&... arguments ) const
	{
		JNI_RETURN_IF_E( !VirtualMachine::IsValid(), TNativeReturnType(), "%s:%d - Attempt to use Uninitialized virtual machine.", __func__, __LINE__ );
		auto local_env	= VirtualMachine::GetLocalEnvironment();

		return Call( local_env, arguments... );
	}

	template< typename TNativeReturnType, typename... TNativeArguments >
	inline TNativeReturnType StaticFunction<TNativeReturnType, TNativeArguments...>::Call( JNIEnv* local_env, const TNativeArguments&... arguments ) const
	{
		JNI_RETURN_IF_E( !IsValid(), TNativeReturnType(), "Function handle was not initialized properly." );

		return FunctionInvocation{ local_env, *m_class_handle, m_function_id }.Call( arguments... );
	}

	template< typename TNativeReturnType, typename... TNativeArguments >
	inline const StaticFunction<TNativeReturnType, TNativeArguments...>&
	StaticFunction<TNativeReturnType, TNativeArguments...>::operator=( const StaticFunction& other )
	{
		m_class_handle	= other.m_class_handle;
		m_function_id	= other.m_function_id;
		return *this;
	}

	template< typename TNativeReturnType, typename... TNativeArguments >
	inline const StaticFunction<TNativeReturnType, TNativeArguments...>&
	StaticFunction<TNativeReturnType, TNativeArguments...>::operator=( StaticFunction&& other )
	{
		m_class_handle	= std::move( other.m_class_handle );
		m_function_id	= std::move( other.m_function_id );
		return *this;
	}
}
