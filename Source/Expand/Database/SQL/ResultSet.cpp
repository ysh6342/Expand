#include "ResultSet.h"
//==============================================
#include "Statement.h"
//==============================================
using namespace Expand::Database::SQL;
//==============================================
ResultSet::ResultSet(std::shared_ptr<Statement>& statement)
: m_statement(statement)
{
}
bool ResultSet::CheckNext()
{
	return m_statement->SetFetch();
}