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

	describe('_combineBindings(bindings)', () => {
		const s = new ProductionSystem([
			'Peter je rodič Jano',
			'Peter je rodič Vlado',
			'manželia Peter Eva',
			'Vlado je rodič Maria',
			'Vlado je rodič Viera',
			'muž Peter',
			'muž Jano',
			'muž Vlado',
			'žena Maria',
			'žena Viera',
			'žena Eva',
		]);
		
		it('should return empty array when no bindings were provided', () => {
			assert.deepEqual(s._combineBindings([]), []);
		});

		it('should return first bindings when only one bindings were provided', () => {
			const bindings = [[{X: 'Peter', Y: 'Jano'}, {X: 'Peter', Y: 'Vlado'}, {X: 'Vlado', Y: 'Maria'}, {X: 'Vlado', Y: 'Viera'}]];
			assert.deepEqual(s._combineBindings(bindings), bindings[0]);
		});

		it('should return valid combinations', () => {
			const bindings = [
				[{X: 'Peter', Y: 'Jano'}, {X: 'Peter', Y: 'Vlado'}, {X: 'Vlado', Y: 'Maria'}, {X: 'Vlado', Y: 'Viera'}],
				[{X: 'Peter', Z: 'Eva'}],
				[{A: 'Maria'}, {A: 'Viera'}, { A: 'Eva'}],
			];
			assert.deepEqual(s._combineBindings(bindings), [
				{X: 'Peter', Y: 'Jano', Z: 'Eva', A: 'Maria'},
				{X: 'Peter', Y: 'Vlado', Z: 'Eva', A: 'Maria'},
				{X: 'Peter', Y: 'Jano', Z: 'Eva', A: 'Viera'},
				{X: 'Peter', Y: 'Vlado', Z: 'Eva', A: 'Viera'},
				{X: 'Peter', Y: 'Jano', Z: 'Eva', A: 'Eva'},
				{X: 'Peter', Y: 'Vlado', Z: 'Eva', A: 'Eva'},
			]);
		});

		it('should pass acceptance test 1', () => {
			const bindings = [
				[{X: 'Peter', Y: 'Jano'}, {X: 'Peter', Y: 'Vlado'}, {X: 'Vlado', Y: 'Maria'}, {X: 'Vlado', Y: 'Viera'}],
				[{Z: 'Peter', X: 'Eva'}],
			];
			assert.deepEqual(s._combineBindings(bindings), []);
		});

		it('should pass acceptance test 2', () => {
			const bindings = [
				[{X: 'Peter', Y: 'Jano'}, {X: 'Peter', Y: 'Vlado'}, {X: 'Vlado', Y: 'Maria'}, {X: 'Vlado', Y: 'Viera'}],
				[{X: 'Peter'}, {X: 'Jano'}, {X: 'Vlado'}],
			];
			assert.deepEqual(s._combineBindings(bindings), [
				{X: 'Peter', Y: 'Jano'},
				{X: 'Peter', Y: 'Vlado'},
				{X: 'Vlado', Y: 'Maria'},
				{X: 'Vlado', Y: 'Viera'},
			]);
		});

		it('should pass acceptance test 3', () => {
			const bindings = [
				[{X: 'Peter', Y: 'Jano'}, {X: 'Peter', Y: 'Vlado'}, {X: 'Vlado', Y: 'Maria'}, {X: 'Vlado', Y: 'Viera'}],
				[{X: 'Maria'}, {X: 'Viera'}, {X: 'Eva'}],
			];
			assert.deepEqual(s._combineBindings(bindings), []);
		});

		it('should pass acceptance test 4', () => {
			const bindings = [
				[{X: 'Peter', Y: 'Jano'}, {X: 'Peter', Y: 'Vlado'}, {X: 'Vlado', Y: 'Maria'}, {X: 'Vlado', Y: 'Viera'}],
				[{X: 'Peter', Z: 'Jano'}, {X: 'Peter', Z: 'Vlado'}, {X: 'Vlado', Z: 'Maria'}, {X: 'Vlado', Z: 'Viera'}],
			];
			assert.deepEqual(s._combineBindings(bindings), [
				{X: 'Peter', Y: 'Jano', Z: 'Jano'},
				{X: 'Peter', Y: 'Vlado', Z: 'Jano'},
				{X: 'Peter', Y: 'Jano', Z: 'Vlado'},
				{X: 'Peter', Y: 'Vlado', Z: 'Vlado'},
				{X: 'Vlado', Y: 'Maria', Z: 'Maria'},
				{X: 'Vlado', Y: 'Viera', Z: 'Maria'},
				{X: 'Vlado', Y: 'Maria', Z: 'Viera'},
				{X: 'Vlado', Y: 'Viera', Z: 'Viera'},
			]);
		});

		it('should pass acceptance test 5', () => {
			const bindings = [
				[],
				[{Y: 'Peter'}, {Y: 'Jano'}, {Y: 'Vlado'}],
			];
			assert.deepEqual(s._combineBindings(bindings), []);
		});

		it('should pass acceptance test 6', () => {
			const bindings = [
				[],
				[{Z: 'Peter', X: 'Jano'}, {Z: 'Peter', X: 'Vlado'}, {Z: 'Vlado', X: 'Maria'}, {Z: 'Vlado', X: 'Viera'}],
			];
			assert.deepEqual(s._combineBindings(bindings), []);
		});

		it('should pass acceptance test 7', () => {
			const bindings = [
				[],
				[{X: 'Maria'}, {X: 'Viera'}, {X: 'Eva'}],
			];
			assert.deepEqual(s._combineBindings(bindings), []);
		});
	});
});
