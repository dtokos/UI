import Parser from './System/Parser';
import ProductionSystem from './System/ProductionSystem';
import Action from './System/Action';

const factsEl = $('textarea[name="facts"]');
const rulesEl = $('textarea[name="rules"]');
const outputEl = $('textarea[name="output"]');

const runBtn = $('#run');
const stepBtn = $('#step');
const resetBtn = $('#reset');

const modal = $('#load-modal');
const programEL = $('input[name="program"]', modal);
const loadBtn = $('.btn-primary', modal);
const progressEl = $('#progress .progress-bar', modal);
const errorsEl = $('#errors', modal);

const parser = new Parser();
const system = new ProductionSystem();
let parsedProgram = {facts: [], rules: []};

runBtn.click(function() {
	system.executeAll();
	renderProgram();
});

stepBtn.click(function() {
	system.executeStep();
	renderProgram();
});

resetBtn.click(function() {
	system.setProgram(parsedProgram);
	renderProgram();
});

modal.on('show.bs.modal', function() {
	hideErrors();
	progressEl.css('width', '0%');
});

loadBtn.click(function() {
	const fileList = programEL.prop('files');
	if (fileList.length == 0) return renderLoadingErrors(['Program musí byť vybraný']);
	if (fileList.length > 1) return renderLoadingErrors(['Iba jeden program môže byť vybraný']);
	
	hideErrors();
	progressEl.css('width', '0%');
	
	const file = fileList[0];
	const reader = new FileReader();
	reader.addEventListener('progress', event => {
		const progress = Math.round(event.loaded / event.total * 100);
		progressEl.css('width', `${progress}%`);
	});
	reader.addEventListener('load', () => {
		parse(reader.result);
	});
	reader.readAsText(file, 'utf-8');
});

const hideErrors = () => errorsEl.hide();
const renderLoadingErrors = messages => {
	$('ul', errorsEl).html(messages.reduce((html, m) => html + `<li>${m}</m>`, ''));
	errorsEl.show();
};

const parse = data => {
	try {
		const program = parser.parse(data);
		parsedProgram = program;
		setProgram(program);
		modal.modal('hide');
	} catch (error) {
		renderLoadingErrors(['Súbor sa nepodarilo zparsovať', error.message]);
	}
};

const setProgram = program => {
	system.setProgram(program);
	renderProgram();
};

const renderProgram = () => {
	renderFacts();
	renderRules();
	renderOutput();
};

const renderFacts = () => {
	factsEl.val(system.memory.reduce((acc, fact) => `${acc}(${fact})\n`, ''));
};

const renderRules = () => {
	rulesEl.val(system.rules.reduce((acc, rule) => `${acc}${renderRule(rule)}\n`, ''));
};

const renderRule = rule => {
	return `${rule.name}:\n` +
		`AK (${rule.if.reduce((acc, cond) => `${acc}(${cond.content})`, '')})\n` +
		`Potom (${rule.then.reduce((acc, act) => `${acc}(${renderActionType(act.type)} ${act.content})`, '')})\n`;
};

const renderActionType = type => {
	switch (type) {
		case Action.types.add: return 'pridaj';
		case Action.types.delete: return 'vymaž';
		case Action.types.message: return 'správa';
	}
};

const renderOutput = () => {
	outputEl.val(system.output.join('\n'));
};
