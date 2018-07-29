#include "Executor.h"
//=====================================
#include "Statement.h"
//=====================================
using namespace Expand::Database::SQL;
//=====================================

//=====================================================================
Executor::Executor(std::shared_ptr<Statement>& statement)
	: m_statement(statement)
{
}

Executor::~Executor()
{
	m_statement->MoreResults();
}
//=====================================================================
DirectExecutor::DirectExecutor(std::shared_ptr<Statement>& statement)
	: m_statement(statement)
{
}

DirectExecutor::~DirectExecutor()
{
	m_statement->CloseCursor();
}
