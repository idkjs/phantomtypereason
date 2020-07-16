type validated;

type unvalidated;

module type FormData = {
  type t('a);
  let create: string => t(unvalidated);
  let validate: t(unvalidated) => t(validated);
  let upperCase: t('a) => t('a);
  let toString: t('a) => string;
};

module FormData: FormData = {
  type t('a) = string;
  let validate = a => a;
  let create = a => a;
  let upperCase = a => String.uppercase_ascii(a);
  let toString = a => a;
};

/* example */
let a = FormData.create("foobarbaz");

let b = FormData.validate(a);

Js.log(b);

Js.log("----------------------------");

Js.log(FormData.upperCase(a));

Js.log("----------------------------");

Js.log(FormData.upperCase(b));

Js.log("----------------------------");
/* will result an error */
/* Js.log(FormData.validate(b)); */
