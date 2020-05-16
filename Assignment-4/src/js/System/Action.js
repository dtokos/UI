const bindingRE = new RegExp(/\?(\w+)/g);
const evalRE = new RegExp(/\{([^\}]*)\}/g);

class Action {
	constructor(type, content) {
		this.type = type;
		this.content = content;
	}

	static get types() {
		return {add: 'add', delete: 'delete', message: 'message'};
	}

	evaluate(binding) {
		const replaced = this.content.replace(bindingRE, (_, variable) => binding[variable]);
		const evaluated = replaced.replace(evalRE, (_, exp) => eval(exp));
		return {type: this.type, content: evaluated};
	}
}

export default Action;
