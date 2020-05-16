import Condition from './Condition';
import Action from './Action';

class ProductionSystem {
	constructor(memory = [], rules = []) {
		this.memory = memory;
		this.rules = rules;
		this.output = [];
	}

	executeStep() {
		const actions = this._findApplicableActions();
		actions.length && this._execute(actions[0]);

		return actions.length != 0;
	}

	executeAll() {
		while (this.executeStep());
	}

	_findApplicableActions() {
		return this.rules.reduce((acc, rule) => acc.concat(this._evaluateRule(rule)), []);
	}

	_evaluateRule(rule) {
		const {success, bindings, special} = this._findBindings(rule.if);
		
		if (!success)
			return [];

		if (bindings.length == 0)
			return this._generate(rule.name, rule.then, [{}]);
		
		const combinedBindings = this._combineBindings(bindings);
		const finalBindings = this._applySpecialRules(combinedBindings, special);
		return this._generate(rule.name, rule.then, finalBindings);
	}

	_findBindings(conditions) {
		const result = {success: true, bindings: [], special: []};
		result.success = conditions.every(cond => {
			switch (cond.type()) {
				case Condition.types.special:
					result.special.push(cond.specialMatch());
					return true;
				case Condition.types.exact:
					return this._exactMatch(cond);
				case Condition.types.predicate:
					const condBinds = this._predicateMatch(cond);
					if (condBinds.length) result.bindings.push(condBinds);
					return condBinds.length > 0;
			}
		});
		return result;
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

	_combineBindings(bindings) {
		if (bindings.length == 0) return [];
		else if (bindings.length == 1) return bindings[0];

		const [first, ...rest] = bindings;

		return rest.reduce((comb, bindings) => {
			return bindings.reduce((newComb, binding) => {
				return newComb.concat(comb.reduce((newBindings, c) => {
					const keys = new Set(Object.keys(c));

					if (!Object.keys(binding).some(bKey => keys.has(bKey) && c[bKey] != binding[bKey]))
						newBindings.push({...c, ...binding});

					return newBindings;
				}, []));
			}, []);
		}, first);
	}

	_applySpecialRules(bindings, rules) {
		return bindings.filter(bind => {
			return rules.every(rule => {
				const vars = rule.variables.map(key => bind[key]);
				return (vars.length == 0 && rule.constants[0] != rule.constants[1]) ||
					(vars.length == 1 && vars[0] != null && vars[0] != rule.constants[0]) ||
					(vars[0] != null && vars[1] != null && vars[0] != vars[1]);
			});
		});
	}

	_generate(name, actions, bindings) {
		return bindings.reduce((result, binding) => {
			const {success, evalActions} = actions.reduce((acc, action) => {
				const evalAction = action.evaluate(binding);
				acc.success = acc.success || this._actionChangesMemory(evalAction);
				acc.evalActions.push(evalAction);
				
				return acc;
			}, {success: false, evalActions: []});

			if (success) result.push({name, actions: evalActions});
			return result;
		}, []);
	}

	_actionChangesMemory(action) {
		switch (action.type) {
			case Action.types.add:
				return !this.memory.includes(action.content);
			case Action.types.delete:
				return this.memory.includes(action.content);
			default:
				return false;
		}
	}

	_execute(actionGroup) {
		actionGroup.actions.forEach(action => {
			switch (action.type) {
				case Action.types.add:
					if (!this.memory.includes(action.content))
						this.memory.push(action.content);
					break;
				case Action.types.delete:
					this.memory = this.memory.filter(fact => fact != action.content);
					break;
				case Action.types.message:
					this.output.push(action.content);
					break;
			}
		});
	}
}

export default ProductionSystem;
