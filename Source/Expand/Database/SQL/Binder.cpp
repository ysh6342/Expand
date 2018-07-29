#include "Binder.h"
//=========================================================
#include "Statement.h"
//=========================================================
using namespace Expand::Database::SQL;

Binder::Binder(std::shared_ptr<Statement>& statement)
	: m_statement(statement)
	, m_bindNum(0)
{

}