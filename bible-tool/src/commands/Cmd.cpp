#include "StdAfx.h"
#include "Cmd.h"

/// -----------------------------------------------------------------------
Cmd::Cmd( const wstring_t& cmdName )
	: cmd_(cmdName)
{

}

/// -----------------------------------------------------------------------
Cmd::~Cmd()
{
}

/// -----------------------------------------------------------------------
wstring_t Cmd::getArgValue( const wstring_t& argName ) const
{
	auto it = args_.find(argName);
	if ( it != args_.end() )
		return it->second;
	return L"";
}

/// -----------------------------------------------------------------------
wstring_t Cmd::getCmdName() const
{
	return cmd_;
}

/// -----------------------------------------------------------------------
bool Cmd::extractArgumets( wstring_t& cmdArgs, const tWStrSet& argNames )
{
	if ( argNames.size() == 0 )
		return true;

	wstring_t argStart(L"--");
	wstring_t::size_type posA = 0, posB;

	while (true) {
		wstring_t cmdName;
		posA = cmdArgs.find(argStart, posA);
		if ( wstring_t::npos == posA ) {
			/// no arguments remains anymore
			break;
		}

		posB = cmdArgs.find(L"=", posA);
		if ( wstring_t::npos == posB ) {
			showHelp();
			return false;
		}

		/// extract argument
		cmdName = cmdArgs.substr(posA + argStart.size(), posB - (posA + argStart.size()));
		trim(cmdName);
		if ( argNames.find(cmdName) == argNames.end() ) {
			std::wcout << cmdName << L" is an invalid argument." << std::endl;
			showHelp();
			return false;
		}

		posA = cmdArgs.find(argStart, posB);
		wstring_t argValue;
		if ( wstring_t::npos == posA )
			argValue = cmdArgs.substr(posB + 1);
		else {
			argValue = cmdArgs.substr(posB + 1, posA - posB - 2);
			posA = posB;
		}
			
		trim(argValue);
		args_.insert( std::make_pair(cmdName, argValue) );
	}
	/*
	for ( size_t i = 0; i < argNames.size() - 1; ++i ) {
		const wstring_t& x = argNames[i];
		const wstring_t& y = argNames[i + 1];
		wstring_t::size_type xpos = cmdArgs.find(x);
		if ( xpos == wstring_t::npos ) {
			showHelp();
			return false;
		}

		cmdArgs.erase( xpos, x.size() + 1 );
		wstring_t::size_type ypos = cmdArgs.find(y);
		if ( ypos == wstring_t::npos ) {
			showHelp();
			return false;
		}

		wstring_t argValue = cmdArgs.substr(xpos, ypos - xpos - 1);
		args_.insert( std::make_pair(x, argValue) );
		cmdArgs.erase(xpos, ypos - xpos);
	}

	/// extract last argument
	const wstring_t& x = argNames[argNames.size() - 1];
	wstring_t::size_type xpos = cmdArgs.find(x);
	if ( xpos == wstring_t::npos ) {
		showHelp();
		return false;
	}
	cmdArgs.erase( xpos, x.size() + 1 );
	args_.insert( std::make_pair(x, cmdArgs) );
	*/
	return true;
}
