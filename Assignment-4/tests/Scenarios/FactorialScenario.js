import assert from 'assert';
import ProductionSystem from '../../src/js/System/ProductionSystem';
import Rule from '../../src/js/System/Rule';
import Condition from '../../src/js/System/Condition';
import Action from '../../src/js/System/Action';

describe('ProductionSystem - FactorialScenario', () => {
	const origMemory = ['faktorial 5'];
	const origRules = [
		new Rule('F1', [new Condition('faktorial 0')], [new Action(Action.types.delete, 'faktorial 0'), new Action(Action.types.add, 'faktorial 0 je 1'), new Action(Action.types.message, 'Faktorial 0 je 1')]),
		new Rule('F2', [new Condition('faktorial ?x'), new Condition('<> ?x 0')], [new Action(Action.types.delete, 'faktorial ?x'), new Action(Action.types.add, 'medzivypocet ?x { ?x - 1 } ?x')]),
		new Rule('F3', [new Condition('medzivypocet ?x 0 ?y')], [new Action(Action.types.delete, 'medzivypocet ?x 0 ?y'), new Action(Action.types.add, 'faktorial ?x je ?y'), new Action(Action.types.message, 'Faktorial ?x je ?y')]),
		new Rule('F4', [new Condition('medzivypocet ?x ?y ?z')], [new Action(Action.types.delete, 'medzivypocet ?x ?y ?z'), new Action(Action.types.add, 'medzivypocet ?x { ?y - 1 } { ?y * ?z }')]),
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

		testStep(1, ['medzivypocet 5 4 5']);
		testStep(2, ['medzivypocet 5 3 20']);
		testStep(3, ['medzivypocet 5 2 60']);
		testStep(4, ['medzivypocet 5 1 120']);
		testStep(5, ['medzivypocet 5 0 120']);
		testStep(6, ['faktorial 5 je 120'], ['Faktorial 5 je 120']);
	});

	describe('executeAll()', () => {
		it('should pass all steps', () => {
			const s = newSystem();
			s.executeAll();
			
			assert.deepEqual(s.rules, origRules);
			assert.deepEqual(s.output, ['Faktorial 5 je 120']);
			assert.deepEqual(s.memory, ['faktorial 5 je 120']);
		});
	});
});
