import assert from 'assert';
import Rule from '../src/js/System/Rule';

describe('Rule', () => {
	describe('constructor()', () => {
		it('should assign name, if, then attributes', () => {
			const r = new Rule('foo', ['if'], ['then']); 
			assert.equal(r.name, 'foo');
			assert.deepEqual(r.if, ['if']);
			assert.deepEqual(r.then, ['then']);
		});
	});
});
