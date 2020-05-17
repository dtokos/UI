import isString from 'lodash/isString';
import isArray from 'lodash/isArray';
import isObject from 'lodash/isObject';

import Rule from './Rule';
import Condition from './Condition';
import Action from './Action';

const actionTypeRE = new RegExp(`^(${Object.values(Action.types).join('|')})`);
const bindingRE = new RegExp(/\?(\w+)/g);

class Parser {
	parse(data) {
		const jsonData = JSON.parse(data);
		this._checkMainKeys(jsonData);
		const facts = this._parseFacts(jsonData.facts);
		const rules = this._parseRules(jsonData.rules);

		return {facts, rules};
	}

	_checkMainKeys(data) {
		if (data.facts == null) throw new Error('No facts were provided');
		if (!isArray(data.facts)) throw new Error('Facts must be an array');
		if (data.rules == null) throw new Error('No rules were provided');
		if (!isArray(data.rules)) throw new Error('Rules must be an array');
	}

	_parseFacts(data) {
		return data.reduce((facts, f) => {
			if (!isString(f)) throw new Error(`Fact must be string: ${f}`);
			
			f = f.trim();
			if (f.length != 0)
				facts.push(f);
			return facts;
		}, []);
	}

	_parseRules(data) {
		return data.map(r => this._parseRule(r));
	}

	_parseRule(data) {
		if (!isObject(data)) throw new Error(`Rule must be object: ${data}`);
		if (!isString(data.name)) throw new Error(`Rule name must be string: ${data.name}`);
		if (!isArray(data.if)) throw new Error(`Rule ifs must be an array: ${data.if}`);
		if (data.if.length == 0) throw new Error('Rule ifs must not be empty');
		if (!isArray(data.then)) throw new Error(`Rule thens must be an array: ${data.then}`);
		if (data.then.length == 0) throw new Error('Rule thens must not be empty');
		
		const name = data.name.trim();
		if (name.length == 0) throw new Error('Rule name must not be empty');

		const conditions = data.if.map(c => this._parseCondition(c));
		const actions = data.then.map(a => this._parseAction(a));

		this._checkBindings(conditions, actions);

		return new Rule(name, conditions, actions);
	}

	_parseCondition(cond) {
		if (!isString(cond)) throw new Error(`Rule if must be string: ${cond}`);
		
		cond = cond.trim();
		if (cond.length == 0) throw new Error('Rule if must not be empty');

		if (cond.startsWith('<>')) this._checkSpecialOperands(cond);

		return new Condition(cond);
	}

	_checkSpecialOperands(cond) {
		const {variables, constants} = new Condition(cond).specialMatch();
		const nOps = variables.length + constants.length;
		if (nOps != 2) throw new Error(`Rule ifs special must contain 2 operands, ${nOps} were given: ${cond}`);
	}
	
	_parseAction(act) {
		if (!isObject(act)) throw new Error(`Rule then must be object: ${act}`);
		if (!isString(act.content)) throw new Error(`Rule then content must be string: ${act.content}`);

		const typeMatch = actionTypeRE.exec(act.type);
		if (typeMatch == null) throw new Error(`Invalid rule then type: ${act.type}`);
		const type = typeMatch[0];

		const content = act.content.trim();
		if (content.length == 0) throw new Error(`Rule then content must not be empty`);

		return new Action(type, content);
	}

	_checkBindings(conds, acts) {
		const condVars = new Set(conds.reduce((acc, c) => acc.concat(c.variables), []));
		const actVars = new Set(acts.reduce((acc, a) => acc.concat([...a.content.matchAll(bindingRE)].map(m => m.slice(1)).flat()), []));
		actVars.forEach(v => {
			if (!condVars.has(v)) throw new Error(`Rule then content uses unbinded variable: ${v}`);
		});
	}
}

export default Parser;