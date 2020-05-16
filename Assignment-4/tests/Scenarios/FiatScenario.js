import assert from 'assert';
import ProductionSystem from '../../src/js/System/ProductionSystem';
import Rule from '../../src/js/System/Rule';
import Condition from '../../src/js/System/Condition';
import Action from '../../src/js/System/Action';

describe('ProductionSystem - FiatScenario', () => {
	const origMemory = ['typ karoserie sedan', 'pocet dveri 4', 'pohanana naprava predna', 'predna maska mriezka', 'ma okruhle svetla'];
	const finalMemory = ['typ karoserie sedan', 'pocet dveri 4', 'pohanana naprava predna', 'predna maska mriezka', 'ma okruhle svetla', 'karoseria sedan', 'sedan 4', 'naprava predna', 'vybrany Fiat Tempra'];
	const origRules = [
		new Rule('FIAT1', [new Condition('typ karoserie ?sedan_hatchback')], [new Action(Action.types.add, 'karoseria ?sedan_hatchback')]),
		new Rule('FIAT2', [new Condition('karoseria sedan'), new Condition('pocet dveri ?4_5')], [new Action(Action.types.add, 'sedan ?4_5')]),
		new Rule('FIAT3', [new Condition('sedan 5')], [new Action(Action.types.add, 'vybrany Fiat Croma'), new Action(Action.types.message, 'Fiat Croma')]),
		new Rule('FIAT4', [new Condition('sedan 4'), new Condition('pohanana naprava ?predna_zadna')], [new Action(Action.types.add, 'naprava ?predna_zadna')]),
		new Rule('FIAT5', [new Condition('naprava predna')], [new Action(Action.types.add, 'vybrany Fiat Tempra'), new Action(Action.types.message, 'Fiat Tempra')]),
		new Rule('FIAT6', [new Condition('naprava zadna')], [new Action(Action.types.add, 'vybrany Fiat Mirafiorri'), new Action(Action.types.message, 'Fiat Mirafiorri')]),
		new Rule('FIAT7', [new Condition('karoseria hatchback'), new Condition('pocet dveri ?3alebo5')], [new Action(Action.types.add, 'hatchback ?3alebo5')]),
		new Rule('FIAT8', [new Condition('hatchback 3'), new Condition('predna maska ?ano_nie_mriezka')], [new Action(Action.types.add, '3 maska ?ano_nie_mriezka')]),
		new Rule('FIAT9', [new Condition('3 maska ano')], [new Action(Action.types.add, 'vybrany Fiat Tipo3'), new Action(Action.types.message, 'Fiat Tipo3')]),
		new Rule('FIAT10', [new Condition('3 maska nie')], [new Action(Action.types.add, 'vybrany Fiat Punto3'), new Action(Action.types.message, 'Fiat Punto3')]),
		new Rule('FIAT11', [new Condition('3 maska mriezka')], [new Action(Action.types.add, 'vybrany Fiat Panda3'), new Action(Action.types.message, 'Fiat Panda3')]),
		new Rule('FIAT12', [new Condition('hatchback 5'), new Condition('predna maska ?ano_nie_mriezka')], [new Action(Action.types.add, '5 maska ?ano_nie_mriezka')]),
		new Rule('FIAT13', [new Condition('5 maska ano')], [new Action(Action.types.add, 'vybrany Fiat Tipo5'), new Action(Action.types.message, 'Fiat Tipo5')]),
		new Rule('FIAT14', [new Condition('5 maska nie')], [new Action(Action.types.add, 'vybrany Fiat Punto5'), new Action(Action.types.message, 'Fiat Punto5')]),
		new Rule('FIAT15', [new Condition('5 maska mriezka'), new Condition('ma ?okruhle_integrovane svetla')],[new Action(Action.types.add, '?okruhle_integrovane svetla')]),
		new Rule('FIAT16', [new Condition('integrovane svetla')], [new Action(Action.types.add, 'vybrany Fiat Uno5'), new Action(Action.types.message, 'Fiat Uno5')]),
		new Rule('FIAT17', [new Condition('okruhle svetla')], [new Action(Action.types.add, 'vybrany Fiat Ritmo5'), new Action(Action.types.message, 'Fiat Ritmo5')]),
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

		testStep(1, finalMemory.slice(0, 6));
		testStep(2, finalMemory.slice(0, 7));
		testStep(3, finalMemory.slice(0, 8));
		testStep(4, finalMemory.slice(0, 9), ['Fiat Tempra']);
	});

	describe('executeAll()', () => {
		it('should pass all steps', () => {
			const s = newSystem();
			s.executeAll();
			
			assert.deepEqual(s.rules, origRules);
			assert.deepEqual(s.output, ['Fiat Tempra']);
			assert.deepEqual(s.memory, finalMemory.slice(0, 9));
		});
	});
});
