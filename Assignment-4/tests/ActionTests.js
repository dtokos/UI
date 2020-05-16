import assert from 'assert';
import Action from '../src/js/System/Action';

describe('Action', () => {
	describe('constructor()', () => {
		it('should assign type and content', () => {
			const r = new Action(Action.types.add, 'foo');
			assert.equal(r.type, Action.types.add);
			assert.equal(r.content, 'foo');
		});
	});

	describe('evaluate(binding)', () => {
		it('should replace variables with bindings', () => {
			const r = new Action(Action.types.add, '?X owns ?Y').evaluate({X: 'Foo', Y: 'Bar'});
			assert.deepEqual(r, {type: Action.types.add, content: 'Foo owns Bar'});
		});

		it('should evaluate math expressions', () => {
			const r = new Action(Action.types.add, 'result ?X {?X - 1}').evaluate({X: '5'});
			assert.deepEqual(r, {type: Action.types.add, content: 'result 5 4'});
		});

		it('should pass acceptance test 1', () => {
			const r = new Action(Action.types.add, 'result ?X {?X + ?X}').evaluate({X: '5'});
			assert.deepEqual(r, {type: Action.types.add, content: 'result 5 10'});
		});

		it('should pass acceptance test 2', () => {
			const r = new Action(Action.types.add, 'result ?X {?X - ?X}').evaluate({X: '5'});
			assert.deepEqual(r, {type: Action.types.add, content: 'result 5 0'});
		});

		it('should pass acceptance test 3', () => {
			const r = new Action(Action.types.add, 'result ?X {?X * ?X}').evaluate({X: '5'});
			assert.deepEqual(r, {type: Action.types.add, content: 'result 5 25'});
		});

		it('should pass acceptance test 4', () => {
			const r = new Action(Action.types.add, 'result ?X {?X / ?X}').evaluate({X: '5'});
			assert.deepEqual(r, {type: Action.types.add, content: 'result 5 1'});
		});
	});
});
