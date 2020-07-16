type option('a) =
  | Some('a)
  | None;

type formData('a);
type validated;
type unvalidated;
let formData: string => formData(unvalidated);
let upperCase: formData(unvalidated) => formData(unvalidated);
let validate: formData(unvalidated) => option(formData(validated));
