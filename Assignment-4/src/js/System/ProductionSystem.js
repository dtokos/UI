import Condition from './Condition';

class ProductionSystem {
	constructor(memory = [], rules = []) {
		this.memory = memory;
		this.rules = rules;
	}

	executeStep() {
		const applicableRules = this._findApplicableRules();
		// TODO: Implement
	}

	expecuteAll() {
		// TODO: Implement
	}

	_findApplicableRules() {
		return this.rules.map(this._evaluateRule);
	}

	_evaluateRule(rule) {
		const bindings = [];
		if (!this._findBindings(bindings, rule.if))
			return;
	}

	_findBindings(bindings, conditions) {
		return conditions.every(cond => {
			switch (cond.type()) {
				case Condition.types.special:
					return true;
				case Condition.types.exact:
					return this._exactMatch(cond);
				case Condition.types.predicate:
					const condBinds = this._predicateMatch(cond);
					if (condBinds.length) bindings.push(condBinds);
					return condBinds.length > 0;
			}
		});
	}

	_exactMatch(cond) {
		return this.memory.some(fact => cond.exactMatch(fact));
	}

	_predicateMatch(cond) {
		return this.memory.reduce((matches, fact) => {
			const binds = cond.predicateMatch(fact);
			if (binds != null)
				matches.push(binds);
			return matches;
		}, []);
	}
}

export default ProductionSystem;
