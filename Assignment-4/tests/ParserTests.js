import assert from 'assert';
import Parser from '../src/js/System/Parser';
import Rule from '../src/js/System/Rule';
import Condition from '../src/js/System/Condition';
import Action from '../src/js/System/Action';

describe('Parser', () => {
	describe('parse(data)', () => {
		const p = new Parser();
		
		it('should throw when given invalid data', () => {
			assert.throws(() => p.parse('foo'), /Unexpected token/);
		});

		it('should throw when missing facts', () => {
			assert.throws(() => p.parse('{"rules": []}'), /No facts were provided/);
		});

		it('should throw when given incorrect facts', () => {
			assert.throws(() => p.parse('{"facts": "foo", "rules": []}'), /Facts must be an array/);
		});

		it('should throw when missing rules', () => {
			assert.throws(() => p.parse('{"facts": []}'), /No rules were provided/);
		});

		it('should throw when given incorrect rules', () => {
			assert.throws(() => p.parse('{"facts": [], "rules": "foo"}'), /Rules must be an array/);
		});

		it('should throw when given facts are not strings', () => {
			assert.throws(() => p.parse('{"facts": ["foo", 1], "rules": []}'), /Fact must be string/);
		});

		it('should throw when given rules are not objects', () => {
			assert.throws(() => p.parse('{"facts": [], "rules": ["foo"]}'), /Rule must be object/);
		});

		it('should throw when given rule name is not string', () => {
			assert.throws(() => p.parse('{"facts": [], "rules": [{"name": 1}]}'), /Rule name must be string/);
		});

		it('should throw when given rule name is empty', () => {
			assert.throws(() => p.parse('{"facts": [], "rules": [{"name": "   ", "if":[1], "then": [1]}]}'), /Rule name must not be empty/);
		});

		it('should throw when given rule ifs are not array', () => {
			assert.throws(() => p.parse('{"facts": [], "rules": [{"name": "foo", "if": 1}]}'), /Rule ifs must be an array/);
		});

		it('should throw when given rule ifs are empty array', () => {
			assert.throws(() => p.parse('{"facts": [], "rules": [{"name": "foo", "if": []}]}'), /Rule ifs must not be empty/);
		});

		it('should throw when given rule thens are not array', () => {
			assert.throws(() => p.parse('{"facts": [], "rules": [{"name": "foo", "if": ["condition"], "then": 1}]}'), /Rule thens must be an array/);
		});

		it('should throw when given rule thens are empty array', () => {
			assert.throws(() => p.parse('{"facts": [], "rules": [{"name": "foo", "if": ["condition"], "then": []}]}'), /Rule thens must not be empty/);
		});

		it('should throw when given rule if is not string', () => {
			assert.throws(() => p.parse('{"facts": [], "rules": [{"name": "foo", "if": [1], "then": [1]}]}'), /Rule if must be string/);
		});

		it('should throw when given rule if is empty', () => {
			assert.throws(() => p.parse('{"facts": [], "rules": [{"name": "foo", "if": ["   "], "then": [1]}]}'), /Rule if must not be empty/);
		});

		it('should throw when given rule then is not object', () => {
			assert.throws(() => p.parse('{"facts": [], "rules": [{"name": "foo", "if": ["foo"], "then": [1]}]}'), /Rule then must be object/);
		});

		it('should throw when given rule then type is invalid', () => {
			assert.throws(() => p.parse('{"facts": [], "rules": [{"name": "foo", "if": ["foo"], "then": [{"type": 1, "content": "foo"}]}]}'), /Invalid rule then type/);
		});

		it('should throw when given rule then content is not string', () => {
			assert.throws(() => p.parse('{"facts": [], "rules": [{"name": "foo", "if": ["foo"], "then": [{"type": "add", "content": 1}]}]}'), /Rule then content must be string/);
		});

		it('should throw when given rule then content is empty', () => {
			assert.throws(() => p.parse('{"facts": [], "rules": [{"name": "foo", "if": ["foo"], "then": [{"type": "add", "content": "   "}]}]}'), /Rule then content must not be empty/);
		});

		it('should throw when bindings dont match', () => {
			assert.throws(() => p.parse('{"facts": [], "rules": [{"name": "foo", "if": ["foo"], "then": [{"type": "add", "content": "?X and ?Y"}]}]}'), /Rule then content uses unbinded variable/);
		});

		it('should parse empty program', () => {
			assert.deepEqual(p.parse('{"facts": [], "rules": []}'), {facts: [], rules: []});
		});

		it('should parse simple program', () => {
			assert.deepEqual(p.parse('{"facts": ["foo owns bar"], "rules": [{"name": "belong", "if": ["?X owns ?Y"], "then": [{"type": "add", "content": "?Y belongs to ?X"}]}]}'), {
				facts: ['foo owns bar'],
				rules: [new Rule('belong', [new Condition('?X owns ?Y')], [new Action(Action.types.add, '?Y belongs to ?X')])],
			});
		});
	});
});
