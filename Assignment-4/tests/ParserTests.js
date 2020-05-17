import assert from 'assert';
import fs from 'fs';
import path from 'path';
import Parser from '../src/js/System/Parser';
import Rule from '../src/js/System/Rule';
import Condition from '../src/js/System/Condition';
import Action from '../src/js/System/Action';

describe('Parser', () => {
	describe('parse(data)', () => {
		const p = new Parser();
		
		it('should throw when given invalid data', () => {
			assert.throws(() => p.parse('foo'), /Unexpected token/);
		});

		it('should throw when missing facts', () => {
			assert.throws(() => p.parse('{"rules": []}'), /No facts were provided/);
		});

		it('should throw when given incorrect facts', () => {
			assert.throws(() => p.parse('{"facts": "foo", "rules": []}'), /Facts must be an array/);
		});

		it('should throw when missing rules', () => {
			assert.throws(() => p.parse('{"facts": []}'), /No rules were provided/);
		});

		it('should throw when given incorrect rules', () => {
			assert.throws(() => p.parse('{"facts": [], "rules": "foo"}'), /Rules must be an array/);
		});

		it('should throw when given facts are not strings', () => {
			assert.throws(() => p.parse('{"facts": ["foo", 1], "rules": []}'), /Fact must be string/);
		});

		it('should throw when given rules are not objects', () => {
			assert.throws(() => p.parse('{"facts": [], "rules": ["foo"]}'), /Rule must be object/);
		});

		it('should throw when given rule name is not string', () => {
			assert.throws(() => p.parse('{"facts": [], "rules": [{"name": 1}]}'), /Rule name must be string/);
		});

		it('should throw when given rule name is empty', () => {
			assert.throws(() => p.parse('{"facts": [], "rules": [{"name": "   ", "if":[1], "then": [1]}]}'), /Rule name must not be empty/);
		});

		it('should throw when given rule ifs are not array', () => {
			assert.throws(() => p.parse('{"facts": [], "rules": [{"name": "foo", "if": 1}]}'), /Rule ifs must be an array/);
		});

		it('should throw when given rule ifs are empty array', () => {
			assert.throws(() => p.parse('{"facts": [], "rules": [{"name": "foo", "if": []}]}'), /Rule ifs must not be empty/);
		});

		it('should throw when given rule thens are not array', () => {
			assert.throws(() => p.parse('{"facts": [], "rules": [{"name": "foo", "if": ["condition"], "then": 1}]}'), /Rule thens must be an array/);
		});

		it('should throw when given rule thens are empty array', () => {
			assert.throws(() => p.parse('{"facts": [], "rules": [{"name": "foo", "if": ["condition"], "then": []}]}'), /Rule thens must not be empty/);
		});

		it('should throw when given rule if is not string', () => {
			assert.throws(() => p.parse('{"facts": [], "rules": [{"name": "foo", "if": [1], "then": [1]}]}'), /Rule if must be string/);
		});

		it('should throw when given rule if is empty', () => {
			assert.throws(() => p.parse('{"facts": [], "rules": [{"name": "foo", "if": ["   "], "then": [1]}]}'), /Rule if must not be empty/);
		});

		it('should throw when given rule if special condition has not 2 operands', () => {
			assert.throws(() => p.parse('{"facts": [], "rules": [{"name": "foo", "if": ["<> 1 2 3"], "then": [1]}]}'), /Rule ifs special must contain 2 operands/);
		});

		it('should throw when given rule then is not object', () => {
			assert.throws(() => p.parse('{"facts": [], "rules": [{"name": "foo", "if": ["foo"], "then": [1]}]}'), /Rule then must be object/);
		});

		it('should throw when given rule then type is invalid', () => {
			assert.throws(() => p.parse('{"facts": [], "rules": [{"name": "foo", "if": ["foo"], "then": [{"type": 1, "content": "foo"}]}]}'), /Invalid rule then type/);
		});

		it('should throw when given rule then content is not string', () => {
			assert.throws(() => p.parse('{"facts": [], "rules": [{"name": "foo", "if": ["foo"], "then": [{"type": "add", "content": 1}]}]}'), /Rule then content must be string/);
		});

		it('should throw when given rule then content is empty', () => {
			assert.throws(() => p.parse('{"facts": [], "rules": [{"name": "foo", "if": ["foo"], "then": [{"type": "add", "content": "   "}]}]}'), /Rule then content must not be empty/);
		});

		it('should throw when bindings dont match', () => {
			assert.throws(() => p.parse('{"facts": [], "rules": [{"name": "foo", "if": ["foo"], "then": [{"type": "add", "content": "?X and ?Y"}]}]}'), /Rule then content uses unbinded variable/);
		});

		it('should parse empty program', () => {
			assert.deepEqual(p.parse('{"facts": [], "rules": []}'), {facts: [], rules: []});
		});

		it('should parse simple program', () => {
			assert.deepEqual(p.parse('{"facts": ["foo owns bar"], "rules": [{"name": "belong", "if": ["?X owns ?Y"], "then": [{"type": "add", "content": "?Y belongs to ?X"}]}]}'), {
				facts: ['foo owns bar'],
				rules: [new Rule('belong', [new Condition('?X owns ?Y')], [new Action(Action.types.add, '?Y belongs to ?X')])],
			});
		});
	});

	describe('acceptance tests', () => {
		const p = new Parser();
		const scenarions = {};
		const readScenario = name => fs.readFileSync(path.resolve(__dirname, '../programs/', name), {encoding:'utf8'});

		before(() => {
			scenarions.fibonacci = readScenario('fibonacci.json');
			scenarions.family = readScenario('family.json');
			scenarions.fiat = readScenario('fiat.json');
			scenarions.factorial = readScenario('factorial.json');
		});

		it('should parse Factorial program', () => {
			assert.deepEqual(p.parse(scenarions.fibonacci), {
				facts: ['fibonacci 7'],
				rules: [
					new Rule('Fi1', [new Condition('fibonacci 1')], [new Action(Action.types.delete, 'fibonacci 1'), new Action(Action.types.add, 'fibonacci 1 je 1'), new Action(Action.types.message, 'fibonacci 1 je 1')]),
					new Rule('Fi2', [new Condition('fibonacci 2')], [new Action(Action.types.delete, 'fibonacci 2'), new Action(Action.types.add, 'fibonacci 2 je 1'), new Action(Action.types.message, 'fibonacci 2 je 1')]),
					new Rule('Fi3', [new Condition('fibonacci ?x'), new Condition('<> ?x 1'), new Condition('<> ?x 2')], [new Action(Action.types.delete, 'fibonacci ?x'), new Action(Action.types.add, 'medzivypocet ?x 1 2 3')]),
					new Rule('Fi4', [new Condition('medzivypocet ?x ?y ?z ?x')], [new Action(Action.types.delete, 'medzivypocet ?x ?y ?z ?x'), new Action(Action.types.add, 'fibonacci ?x je ?z'), new Action(Action.types.message, 'fibonacci ?x je ?z')]),
					new Rule('Fi5', [new Condition('medzivypocet ?x ?y ?z ?u')], [new Action(Action.types.delete, 'medzivypocet ?x ?y ?z ?u'), new Action(Action.types.add, 'medzivypocet ?x ?z { ?y + ?z } { ?u + 1 }')]),
				],
			});
		});

		it('should parse Family program', () => {
			assert.deepEqual(p.parse(scenarions.family), {
				facts: [
					'Peter je rodič Jano', 'Peter je rodič Vlado', 'manželia Peter Eva',
					'Vlado je rodič Maria', 'Vlado je rodič Viera', 'muž Peter',
					'muž Jano', 'muž Vlado', 'žena Maria',
					'žena Viera', 'žena Eva',
				],
				rules: [
					new Rule('Druhyrodič1', [new Condition('?X je rodič ?Y'), new Condition('manželia ?X ?Z')], [new Action(Action.types.add, '?Z je rodič ?Y')]),
					new Rule('Druhyrodič2', [new Condition('?X je rodič ?Y'), new Condition('manželia ?Z ?X')], [new Action(Action.types.add, '?Z je rodič ?Y')]),
					new Rule('Otec', [new Condition('?X je rodič ?Y'), new Condition('muž ?X')], [new Action(Action.types.add, '?X je otec ?Y')]),
					new Rule('Matka', [new Condition('?X je rodič ?Y'), new Condition('žena ?X')], [new Action(Action.types.add, '?X je matka ?Y')]),
					new Rule('Surodenci', [new Condition('?X je rodič ?Y'), new Condition('?X je rodič ?Z'), new Condition('<> ?Y ?Z')], [new Action(Action.types.add, '?Y a ?Z su surodenci')]),
					new Rule('Brat', [new Condition('?Y a ?Z su surodenci'), new Condition('muž ?Y')], [new Action(Action.types.add, '?Y je brat ?Z')]),
					new Rule('Stryko', [new Condition('?Y je brat ?Z'), new Condition('?Z je rodič ?X')], [new Action(Action.types.add, '?Y je stryko ?X'), new Action(Action.types.message, '?X ma stryka')]),
					new Rule('Test mazania', [new Condition('?Y je stryko ?X'), new Condition('žena ?X')], [new Action(Action.types.delete, 'žena ?X')]),
				],
			});
		});

		it('should parse Fiat program', () => {
			assert.deepEqual(p.parse(scenarions.fiat), {
				facts: ['typ karoserie sedan', 'pocet dveri 4', 'pohanana naprava predna', 'predna maska mriezka', 'ma okruhle svetla'],
				rules: [
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
				],
			});
		});

		it('should parse Fibonacci program', () => {
			assert.deepEqual(p.parse(scenarions.factorial), {
				facts: ['faktorial 5'],
				rules: [
					new Rule('F1', [new Condition('faktorial 0')], [new Action(Action.types.delete, 'faktorial 0'), new Action(Action.types.add, 'faktorial 0 je 1'), new Action(Action.types.message, 'Faktorial 0 je 1')]),
					new Rule('F2', [new Condition('faktorial ?x'), new Condition('<> ?x 0')], [new Action(Action.types.delete, 'faktorial ?x'), new Action(Action.types.add, 'medzivypocet ?x { ?x - 1 } ?x')]),
					new Rule('F3', [new Condition('medzivypocet ?x 0 ?y')], [new Action(Action.types.delete, 'medzivypocet ?x 0 ?y'), new Action(Action.types.add, 'faktorial ?x je ?y'), new Action(Action.types.message, 'Faktorial ?x je ?y')]),
					new Rule('F4', [new Condition('medzivypocet ?x ?y ?z')], [new Action(Action.types.delete, 'medzivypocet ?x ?y ?z'), new Action(Action.types.add, 'medzivypocet ?x { ?y - 1 } { ?y * ?z }')]),
				],
			});
		});
	});
});
