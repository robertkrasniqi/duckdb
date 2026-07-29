//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/planner/operator/logical_extension_operator.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/planner/logical_operator.hpp"
#include "duckdb/planner/operator_extension.hpp"

namespace duckdb {

class ColumnBindingResolver;
class FlattenDependentJoins;

struct LogicalExtensionOperator : public LogicalOperator {
public:
	static constexpr const LogicalOperatorType TYPE = LogicalOperatorType::LOGICAL_EXTENSION_OPERATOR;

public:
	LogicalExtensionOperator() : LogicalOperator(LogicalOperatorType::LOGICAL_EXTENSION_OPERATOR) {
	}
	explicit LogicalExtensionOperator(vector<unique_ptr<Expression>> expressions)
	    : LogicalOperator(LogicalOperatorType::LOGICAL_EXTENSION_OPERATOR, std::move(expressions)) {
	}

	void Serialize(Serializer &serializer) const override;
	static unique_ptr<LogicalOperator> Deserialize(Deserializer &deserializer);

	virtual PhysicalOperator &CreatePlan(ClientContext &context, PhysicalPlanGenerator &planner) = 0;

	virtual void ResolveColumnBindings(ColumnBindingResolver &res, vector<ColumnBinding> &bindings);
	virtual string GetExtensionName() const;

	// Whether the optimizer must treat all of this operator's input columns as referenced
	virtual bool RequiresAllColumns() const;

	// correlation hook
	virtual vector<ColumnBinding> PushdownDependentJoin(FlattenDependentJoins &flattener,
	                                                    unique_ptr<LogicalOperator> &plan, bool propagate_null_values,
	                                                    vector<ColumnBinding> state);

protected:
	// Helper for PushdownDependentJoin, calls FlattenDependentJoins::PushDownChild
	static vector<ColumnBinding> PushDownDependentJoinChild(FlattenDependentJoins &flattener,
	                                                        unique_ptr<LogicalOperator> &plan,
	                                                        bool propagate_null_values, vector<ColumnBinding> state,
	                                                        idx_t child_idx = 0, bool rewrite_parent = true);
};
} // namespace duckdb
