import assert from 'assert';
import ProductionSystem from '../../src/js/System/ProductionSystem';
import Rule from '../../src/js/System/Rule';
import Condition from '../../src/js/System/Condition';
import Action from '../../src/js/System/Action';

describe('ProductionSystem - FamilyScenario', () => {
	const origMemory = [
		'Peter je rodič Jano', 'Peter je rodič Vlado', 'manželia Peter Eva',
		'Vlado je rodič Maria', 'Vlado je rodič Viera', 'muž Peter',
		'muž Jano', 'muž Vlado', 'žena Maria',
		'žena Viera', 'žena Eva',
	];
	const finalMemory = [
		'Peter je rodič Jano', 'Peter je rodič Vlado', 'manželia Peter Eva',
		'Vlado je rodič Maria', 'Vlado je rodič Viera', 'muž Peter',
		'muž Jano', 'muž Vlado', 'žena Eva',
		'Eva je rodič Jano', 'Eva je rodič Vlado', 'Peter je otec Jano',
		'Peter je otec Vlado', 'Vlado je otec Maria', 'Vlado je otec Viera',
		'Eva je matka Jano', 'Eva je matka Vlado', 'Vlado a Jano su surodenci',
		'Jano a Vlado su surodenci', 'Viera a Maria su surodenci', 'Maria a Viera su surodenci',
		'Jano je brat Vlado', 'Vlado je brat Jano', 'Jano je stryko Maria',
		'Jano je stryko Viera',
	];
	const finalOutput = ['Maria ma stryka', 'Viera ma stryka'];
	const origRules = [
		new Rule('Druhyrodič1', [new Condition('?X je rodič ?Y'), new Condition('manželia ?X ?Z')], [new Action(Action.types.add, '?Z je rodič ?Y')]),
		new Rule('Druhyrodič2', [new Condition('?X je rodič ?Y'), new Condition('manželia ?Z ?X')], [new Action(Action.types.add, '?Z je rodič ?Y')]),
		new Rule('Otec', [new Condition('?X je rodič ?Y'), new Condition('muž ?X')], [new Action(Action.types.add, '?X je otec ?Y')]),
		new Rule('Matka', [new Condition('?X je rodič ?Y'), new Condition('žena ?X')], [new Action(Action.types.add, '?X je matka ?Y')]),
		new Rule('Surodenci', [new Condition('?X je rodič ?Y'), new Condition('?X je rodič ?Z'), new Condition('<> ?Y ?Z')], [new Action(Action.types.add, '?Y a ?Z su surodenci')]),
		new Rule('Brat', [new Condition('?Y a ?Z su surodenci'), new Condition('muž ?Y')], [new Action(Action.types.add, '?Y je brat ?Z')]),
		new Rule('Stryko', [new Condition('?Y je brat ?Z'), new Condition('?Z je rodič ?X')], [new Action(Action.types.add, '?Y je stryko ?X'), new Action(Action.types.message, '?X ma stryka')]),
		new Rule('Test mazania', [new Condition('?Y je stryko ?X'), new Condition('žena ?X')], [new Action(Action.types.delete, 'žena ?X')]),
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

		testStep(1, [...origMemory, ...finalMemory.slice(9, 10)]);
		testStep(2, [...origMemory, ...finalMemory.slice(9, 11)]);
		testStep(3, [...origMemory, ...finalMemory.slice(9, 12)]);
		testStep(4, [...origMemory, ...finalMemory.slice(9, 13)]);
		testStep(5, [...origMemory, ...finalMemory.slice(9, 14)]);
		testStep(6, [...origMemory, ...finalMemory.slice(9, 15)]);
		testStep(7, [...origMemory, ...finalMemory.slice(9, 16)]);
		testStep(8, [...origMemory, ...finalMemory.slice(9, 17)]);
		testStep(9, [...origMemory, ...finalMemory.slice(9, 18)]);
		testStep(10, [...origMemory, ...finalMemory.slice(9, 19)]);
		testStep(11, [...origMemory, ...finalMemory.slice(9, 20)]);
		testStep(12, [...origMemory, ...finalMemory.slice(9, 21)]);
		testStep(13, [...origMemory, ...finalMemory.slice(9, 22)]);
		testStep(14, [...origMemory, ...finalMemory.slice(9, 23)]);
		testStep(15, [...origMemory, ...finalMemory.slice(9, 24)], finalOutput.slice(0, 1));
		testStep(16, [...origMemory, ...finalMemory.slice(9, 25)], finalOutput);
		testStep(17, [...origMemory.slice(0, 8), ...origMemory.slice(9), ...finalMemory.slice(9, 25)], finalOutput);
		testStep(18, [...origMemory.slice(0, 8), ...origMemory.slice(10), ...finalMemory.slice(9, 25)], finalOutput);
		testStep(19, finalMemory, finalOutput);
	});

	describe('executeAll()', () => {
		it('should pass all steps', () => {
			const s = newSystem();
			s.executeAll();
			
			assert.deepEqual(s.rules, origRules);
			assert.deepEqual(s.output, finalOutput);
			assert.deepEqual(s.memory, finalMemory);
		});
	});
});
