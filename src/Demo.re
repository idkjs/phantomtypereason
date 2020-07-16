Js.log("Hello, BuckleScript and Reason!");

open !FormData;

let a: formData(unvalidated) = formData("foobar");
let b: option(formData(validated)) = validate(a);
let c = upperCase(formData("foobar"));

Js.log2("formData(unvalidated): ",a);
Js.log2("option(formData(validated)): ",b);
Js.log2("upperCase foobar: ",c);

let a: formData(unvalidated) = formData("foobar");
let b: option(formData(validated)) = validate(a);
// let c =
//    switch (b) {
//    | Some(a) => FormData.upperCase(a)
//    | None => a
//    }

//    Js.log2("upperCase foobar: ",c);