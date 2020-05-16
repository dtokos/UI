import assert from 'assert';
import Condition from '../src/js/System/Condition';

describe('Condition', () => {
	describe('constructor()', () => {
		it('should assign content attribute', () => {
			assert.equal(new Condition('content').content, 'content');
		});

		it('should assign empty variables', () => {
			assert.deepEqual(new Condition('content').variables, []);
		});

		it('should assign parsed variables', () => {
			assert.deepEqual(new Condition('?X is ?Y').variables, ['X', 'Y']);
		});

		it('should assign empty pattern', () => {
			assert.equal(new Condition('content').pattern, null);
		});

		it('should assign parsed pattern', () => {
			assert.equal(new Condition('?X is ?Y').pattern.source, /^(\w+) is (\w+)$/.source);
		});
	});

	describe('type()', () => {
		it('should return special type', () => {
			assert.equal(new Condition('<> ?X ?Y').type(), Condition.types.special);
		});

		it('should return exact type', () => {
			assert.equal(new Condition('foo 5').type(), Condition.types.exact);
		});

		it('should return predicate type', () => {
			assert.equal(new Condition('?X is ?Y').type(), Condition.types.predicate);
		});
	});

	describe('exactMatch(fact)', () => {
		it('should return true', () => {
			assert.ok(new Condition('foo 5').exactMatch('foo 5'));
		});

		it('should return false', () => {
			assert.ok(!new Condition('foo 5').exactMatch('bar 4'));
		});
	});

	describe('predicateMatch(fact)', () => {
		it('should return object with variables', () => {
			assert.deepEqual(new Condition('?X is less than ?Y').predicateMatch('5 is less than 6'), {X: 5, Y: 6});
		});

		it('should return null when it doesnt match', () => {
			assert.equal(new Condition('?X is less than ?Y').predicateMatch('foo bar'), null);
		});
	});

	describe('specialMatch()', () => {
		it('should return array with variables', () => {
			assert.deepEqual(new Condition('<> ?X ?Y').specialMatch(), {variables: ['X', 'Y'], constants: []});
		});

		it('should return array with constants', () => {
			assert.deepEqual(new Condition('<> 1 0').specialMatch(), {variables: [], constants: ['1', '0']});
		});

		it('should return array with variables and constants', () => {
			assert.deepEqual(new Condition('<> ?X 0').specialMatch(), {variables: ['X'], constants: ['0']});
		});

		it('should return empty array when no variables were provided', () => {
			assert.deepEqual(new Condition('<>').specialMatch(), {variables: [], constants: []});
		});
	});
});
