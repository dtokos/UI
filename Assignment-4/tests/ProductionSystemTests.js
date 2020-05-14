import assert from 'assert';
import ProductionSystem from '../src/js/System/ProductionSystem';
import Rule from '../src/js/System/Rule';
import Condition from '../src/js/System/Condition';

describe('ProductionSystem', () => {
	describe('constructor()', () => {
		it('should assign memory and rules with default values if not provided', () => {
			const s = new ProductionSystem();
			assert.deepEqual(s.memory, []);
			assert.deepEqual(s.rules, []);
		});

		it('should assign memory and rules attributes', () => {
			const s = new ProductionSystem(['memory'], ['rules']);
			assert.deepEqual(s.memory, ['memory']);
			assert.deepEqual(s.rules, ['rules']);
		});
	});

	describe('_findBindings(bindings, conditions)', () => {
		it('should skip exact matches', () => {
			const bindings = [];
			const s = new ProductionSystem(['foo is not bar']);
			assert.ok(s._findBindings(bindings, [new Condition('foo is not bar')]));
			assert.equal(bindings.length, 0);
		});

		it('should fail when there is no exact match', () => {
			const bindings = [];
			const s = new ProductionSystem(['foo is not bar']);
			assert.ok(!s._findBindings(bindings, [new Condition('bar is not foo')]));
			assert.equal(bindings.length, 0);
		});

		it('should skip special conditions', () => {
			const bindings = [];
			const s = new ProductionSystem(['foo is not bar']);
			assert.ok(s._findBindings(bindings, [new Condition('<> ?X ?Y')]));
			assert.equal(bindings.length, 0);
		});

		it('should parse bindings', () => {
			const bindings = [];
			const s = new ProductionSystem(['foo is not bar'], ['other fact']);
			assert.ok(s._findBindings(bindings, [new Condition('?X is not ?Y')]));
			assert.deepEqual(bindings, [[{X: 'foo', Y: 'bar'}]]);
		});
	});
});
