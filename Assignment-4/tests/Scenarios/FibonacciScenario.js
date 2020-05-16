import assert from 'assert';
import ProductionSystem from '../../src/js/System/ProductionSystem';
import Rule from '../../src/js/System/Rule';
import Condition from '../../src/js/System/Condition';
import Action from '../../src/js/System/Action';

describe('ProductionSystem - FibonacciScenario', () => {
	const origMemory = ['fibonacci 7'];
	const origRules = [
		new Rule('Fi1', [new Condition('fibonacci 1')], [new Action(Action.types.delete, 'fibonacci 1'), new Action(Action.types.add, 'fibonacci 1 je 1'), new Action(Action.types.message, 'fibonacci 1 je 1')]),
		new Rule('Fi2', [new Condition('fibonacci 2')], [new Action(Action.types.delete, 'fibonacci 2'), new Action(Action.types.add, 'fibonacci 2 je 1'), new Action(Action.types.message, 'fibonacci 2 je 1')]),
		new Rule('Fi3', [new Condition('fibonacci ?x'), new Condition('<> ?x 1'), new Condition('<> ?x 2')], [new Action(Action.types.delete, 'fibonacci ?x'), new Action(Action.types.add, 'medzivypocet ?x 1 2 3')]),
		new Rule('Fi4', [new Condition('medzivypocet ?x ?y ?z ?x')], [new Action(Action.types.delete, 'medzivypocet ?x ?y ?z ?x'), new Action(Action.types.add, 'fibonacci ?x je ?z'), new Action(Action.types.message, 'fibonacci ?x je ?z')]),
		new Rule('Fi5', [new Condition('medzivypocet ?x ?y ?z ?u')], [new Action(Action.types.delete, 'medzivypocet ?x ?y ?z ?u'), new Action(Action.types.add, 'medzivypocet ?x ?z { ?y + ?z } { ?u + 1 }')]),
	];
	const newSystem = () => new ProductionSystem([...origMemory], [...origRules]);

	describe('executeStep()', () => {
		const testStep = (iterations, memory, output = []) => {
			it(`should pass step ${iterations}`, () => {
				const s = newSystem();
				for (let i = 0; i < iterations; i++)
					s.executeStep();
				
				assert.deepEqual(s.rules, origRules);
				assert.deepEqual(s.output, output);
				assert.deepEqual(s.memory, memory);
			});
		};

		testStep(1, ['medzivypocet 7 1 2 3']);
		testStep(2, ['medzivypocet 7 2 3 4']);
		testStep(3, ['medzivypocet 7 3 5 5']);
		testStep(4, ['medzivypocet 7 5 8 6']);
		testStep(5, ['medzivypocet 7 8 13 7']);
		testStep(6, ['fibonacci 7 je 13'], ['fibonacci 7 je 13']);
	});

	describe('executeAll()', () => {
		it('should pass all steps', () => {
			const s = newSystem();
			s.executeAll();
			
			assert.deepEqual(s.rules, origRules);
			assert.deepEqual(s.output, ['fibonacci 7 je 13']);
			assert.deepEqual(s.memory, ['fibonacci 7 je 13']);
		});
	});
});
