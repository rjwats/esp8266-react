export default (validator1, validator2) => value => validator1(value) || validator2(value);
