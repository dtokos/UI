import assert from 'assert';
import ProductionSystem from '../src/js/System/ProductionSystem';
import Rule from '../src/js/System/Rule';
import Condition from '../src/js/System/Condition';
import Action from '../src/js/System/Action';

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
			const s = new ProductionSystem(['foo is not bar']);
			const r = s._findBindings([new Condition('foo is not bar')]);
			assert.deepEqual(r, {success: true, bindings: [], special: []});
		});

		it('should fail when there is no exact match', () => {
			const s = new ProductionSystem(['foo is not bar']);
			const r = s._findBindings([new Condition('bar is not foo')]);
			assert.deepEqual(r, {success: false, bindings: [], special: []});
		});

		it('should parse special conditions', () => {
			const s = new ProductionSystem(['foo is not bar']);
			const r = s._findBindings([new Condition('<> ?X ?Y')]);
			assert.deepEqual(r, {success: true, bindings: [], special: [{variables: ['X', 'Y'], constants: []}]});
		});

		it('should parse bindings', () => {
			const s = new ProductionSystem(['foo is not bar'], ['other fact']);
			const r = s._findBindings([new Condition('?X is not ?Y')]);
			assert.deepEqual(r, {success: true, bindings: [[{X: 'foo', Y: 'bar'}]], special: []});
		});

		it('should pass acceptance test', () => {
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
			const r = s._findBindings([new Condition('?X je rodič ?Y'), new Condition('?X je rodič ?Z'), new Condition('<> ?Y ?Z')]);
			assert.deepEqual(r, {
				success: true,
				bindings: [
					[{X: 'Peter', Y: 'Jano'}, {X: 'Peter', Y: 'Vlado'}, {X: 'Vlado', Y: 'Maria'}, {X: 'Vlado', Y: 'Viera'}],
					[{X: 'Peter', Z: 'Jano'}, {X: 'Peter', Z: 'Vlado'}, {X: 'Vlado', Z: 'Maria'}, {X: 'Vlado', Z: 'Viera'}],
				],
				special: [{variables: ['Y', 'Z'], constants: []}],
			});
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

	describe('_applySpecialRules(bindings, rules)', () => {
		it('should keep binding when it passes all rules', () => {
			const s = new ProductionSystem();
			const r = s._applySpecialRules([
				{X: 'Peter', Y: 'Jano', Z: 'Jano'},
				{X: 'Peter', Y: 'Vlado', Z: 'Jano'},
				{X: 'Peter', Y: 'Jano', Z: 'Vlado'},
				{X: 'Peter', Y: 'Vlado', Z: 'Vlado'},
				{X: 'Vlado', Y: 'Maria', Z: 'Maria'},
				{X: 'Vlado', Y: 'Viera', Z: 'Maria'},
				{X: 'Vlado', Y: 'Maria', Z: 'Viera'},
				{X: 'Vlado', Y: 'Viera', Z: 'Viera'},
			], [{variables: ['Y', 'Z'], constants: []}]);
			assert.deepEqual(r, [
				{X: 'Peter', Y: 'Vlado', Z: 'Jano'},
				{X: 'Peter', Y: 'Jano', Z: 'Vlado'},
				{X: 'Vlado', Y: 'Viera', Z: 'Maria'},
				{X: 'Vlado', Y: 'Maria', Z: 'Viera'},
			]);
		});

		it('should return empty array when all bindings fail rules', () => {
			const s = new ProductionSystem();
			const r = s._applySpecialRules([
				{X: 'Peter', Y: 'Jano', Z: 'Jano'},
				{X: 'Peter', Y: 'Vlado', Z: 'Vlado'},
				{X: 'Vlado', Y: 'Maria', Z: 'Maria'},
				{X: 'Vlado', Y: 'Viera', Z: 'Viera'},
			], [{variables: ['Y', 'Z'], constants: []}]);
			assert.deepEqual(r, []);
		});

		it('should pass acceptance test 1', () => {
			const s = new ProductionSystem();
			const r = s._applySpecialRules([
				{X: '1'}, {X: '2'}, {X: '3'}, {X: '4'},
			], [{variables: ['X'], constants: ['2']}]);
			assert.deepEqual(r, [
				{X: '1'}, {X: '3'}, {X: '4'}
			]);
		});
	});

	describe('_generate(name, actions, bindings)', () => {
		it('it should create new actions', () => {
			const s = new ProductionSystem();
			const r = s._generate('ownership', [new Action(Action.types.add, '?X owns ?Y')], [{X: 'Foo', Y: 'Bar'}, {X: 'Foo', Y: 'Baz'}]);
			assert.deepEqual(r, [
				{name: 'ownership', actions: [{type: Action.types.add, content: 'Foo owns Bar'}]},
				{name: 'ownership', actions: [{type: Action.types.add, content: 'Foo owns Baz'}]},
			]);
		});

		it('should create new actions with evaluated expressions', () => {
			const s = new ProductionSystem();
			const r = s._generate('addition', [new Action(Action.types.add, 'result ?X {?X + ?Y}')], [{X: '1', Y: '2'}, {X: '3', Y: '4'}]);
			assert.deepEqual(r, [
				{name: 'addition', actions: [{type: Action.types.add, content: 'result 1 3'}]},
				{name: 'addition', actions: [{type: Action.types.add, content: 'result 3 7'}]},
			]);
		});

		it('should not create actions that doesnt change memory', () => {
			const s = new ProductionSystem(['fact foo bar']);
			const r1 = s._generate('facts', [new Action(Action.types.add, 'fact ?X ?Y')], [{X: 'foo', Y: 'bar'}]);
			const r2 = s._generate('facts', [new Action(Action.types.delete, 'fact ?X ?Y')], [{X: 'bar', Y: 'foo'}]);
			const r3 = s._generate('facts', [new Action(Action.types.message, 'fact ?X ?Y')], [{X: 'bar', Y: 'foo'}]);
			assert.deepEqual(r1, []);
			assert.deepEqual(r2, []);
			assert.deepEqual(r3, []);
		});

		it('should keep all actions if at least one changes memory', () => {
			const s = new ProductionSystem();
			const r = s._generate('addition', [new Action(Action.types.add, 'result ?X {?X + ?Y}'), new Action(Action.types.message, 'added ?X ?Y')], [{X: '1', Y: '2'}, {X: '3', Y: '4'}]);
			assert.deepEqual(r, [
				{name: 'addition', actions: [{type: Action.types.add, content: 'result 1 3'}, {type: Action.types.message, content: 'added 1 2'}]},
				{name: 'addition', actions: [{type: Action.types.add, content: 'result 3 7'}, {type: Action.types.message, content: 'added 3 4'}]},
			]);
		});
	});
});
