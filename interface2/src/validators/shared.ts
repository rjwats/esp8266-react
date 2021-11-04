import Schema, { ValidateOption } from "async-validator";

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
