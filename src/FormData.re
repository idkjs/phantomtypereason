type option('a) =
  | Some('a)
  | None;

type formData('a) = string;
type validated;
type unvalidated;
let formData = a => a;
let upperCase = a => String.uppercase_ascii(a);
let validate = a =>
  if (String.length(a) > 3) {
    Some(a);
  } else {
    None;
  };
