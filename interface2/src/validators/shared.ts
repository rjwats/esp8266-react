import Schema, { InternalRuleItem, ValidateOption } from "async-validator";

export const validate = <T extends object>(validator: Schema, source: Partial<T>, options?: ValidateOption): Promise<T> => {
  return new Promise(
    (resolve, reject) => {
      validator.validate(source, options ? options : {}, (errors, fieldErrors) => {
        if (errors) {
          reject(fieldErrors);
        } else {
          resolve(source as T);
        }
      });
    }
  );
};

// eslint-disable-next-line max-len
const IP_ADDRESS_REGEXP = /^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/;

export const IP_ADDRESS_VALIDATOR = {
  validator(rule: InternalRuleItem, value: string, callback: (error?: string) => void) {
    if (value && !IP_ADDRESS_REGEXP.test(value)) {
      callback("Must be an IP address");
    } else {
      callback();
    }
  }
};

const HOSTNAME_LENGTH_REGEXP = /^.{0,63}$/;
const HOSTNAME_PATTERN_REGEXP = /^(([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9-]*[a-zA-Z0-9])\.)*([A-Za-z0-9]|[A-Za-z0-9][A-Za-z0-9-]*[A-Za-z0-9])$/;

export const HOSTNAME_VALIDATOR = {
  validator(rule: InternalRuleItem, value: string, callback: (error?: string) => void) {
    if (value && (!HOSTNAME_LENGTH_REGEXP.test(value) || !HOSTNAME_PATTERN_REGEXP.test(value))) {
      callback("Must be an IP address");
    } else {
      callback();
    }
  }
};
