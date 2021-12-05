import Schema, { InternalRuleItem } from "async-validator";
import { Palette } from "./types";

export const createPaletteValidator = (palettes: Palette[], creating: boolean) => new Schema({
  name: [
    { required: true, message: "Name is required" },
    { type: "string", pattern: /^[a-zA-Z0-9 ]{1,24}$/, message: "Must be 1-24 characters: alpha numeric" },
    ...(creating ? [createUniqueNameValidator(palettes)] : [])
  ]
});

export const createUniqueNameValidator = (palettes: Palette[]) => ({
  validator(rule: InternalRuleItem, name: string, callback: (error?: string) => void) {
    if (palettes && palettes.find((p) => p.name === name)) {
      callback("Name already in use");
    } else {
      callback();
    }
  }
});
