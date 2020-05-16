const variablesRE = new RegExp(/\?(\w+)(?= |$)/, 'g');

class Condition {
	constructor(content) {
		this.content = content;
		this.variables = this._match(variablesRE, content);
		this.pattern = this.variables.length ? new RegExp(`^${content.replace(variablesRE, '(\\w+)')}$`) : null;
	}

	static get types() {
		return {special: 'special', exact: 'exact', predicate: 'predicate'};
	}

	type() {
		if (this.content.startsWith('<>')) return Condition.types.special;
		else if (this.content.includes('?')) return Condition.types.predicate;
		else return Condition.types.exact;
	}

	exactMatch(fact) {
		return this.content == fact;
	}

	predicateMatch(fact) {
		const values = this._match(this.pattern, fact, false);
		if (values == null || values.length == 0 || values.length != this.variables.length)
			return null;

		return this.variables.reduce((obj, v, i) => {
			obj[v] = values[i];
			return obj;
		}, {});
	}

	specialMatch() {
		return this.content.replace(/\s+/g, ' ')
			.trim()
			.split(' ')
			.slice(1)
			.reduce((acc, item) => {
				if (item.startsWith('?')) acc.variables.push(item.substring(1));
				else acc.constants.push(item);
				return acc;
			}, {variables: [], constants: []});
	}

	_match(pattern, value, all = true) {
		if (all) return [...value.matchAll(pattern)].map(m => m.slice(1)).flat();
		const matches = value.match(pattern);
		return matches && matches.slice(1);
	}
}

export default Condition;
