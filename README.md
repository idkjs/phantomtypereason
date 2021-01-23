# [Phantom types in ReasonML](https://gist.github.com/busypeoples/3a28d039272ec3eb33ca2fc6b32dafc7)

### Introduction

"A phantom type is a parametrised type whose parameters do not all appear on the right-hand side of its definition..."
[Haskell Wiki, PhantomType](https://wiki.haskell.org/Phantom_type)

The following write-up is intended as an introduction into using phantom types in **ReasonML**.

Taking a look at the above definition from the Haskell wiki, it states that phantom types are parametrised types where not all parameters appear on the right-hand side. Let's try to see if we can implement a similar example as in said wiki.

#### Updated to ReasonML 3.1 (09.03.2018)

### Example

```reason
type formData('a) = string;
```

So `formData` is a phantom type as the `'a` parameter only appears on the left side.

Let's create file `FormData.rei` to define the `formData` interface and add the above definition.

We want to enable a library user to create a `formData`.
What we also want is restrict the type in certain parts of the library.
For example we want to be able to differentiate between validated und unvalidated form data. So let's add two type definitions `validated` and `unvalidated` to our `FormData.rei` file.

```reason
type validated;
type unvalidated;
```

Nothing special up until here, just two lone type definitions.
So for example we want to expose a function, that receives a string and returns an unvalidated `formData` type, which we will do by extending our API.

```reason
let formData: string => formData(unvalidated);
```

And maybe we want to add an `upperCase` function that does exactly that, take a unvalidated input and return an unvalidated input, so again let's update our API.

```reason
let upperCase: formData(unvalidated) => formData(unvalidated);
```

Finally let's also add a `validate` function that either returns nothing or the validated input.

```reason
type option('a) =
  | Some('a)
  | None;

let validate: formData(unvalidated) => option(formData(validated));
```

Next, we'll create a new file and name it `FormData.re`, where we will implement the interface.

```reason
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
```

Let's try to use the library:

```reason
open FormData;

let a: formData(unvalidated) = formData("foobar");
let b: option(formData(validated)) = validate(a);
```

So we can see that this works as expected. We validate "foobar" and get the validated form input in return.
But what if we wanted to call the `upperCase` function?

```reason
let c =
   switch (b) {
   | Some(a) => FormData.upperCase(a)
   | None => a
   };
```

The compiler will complain:

```
Error: This expression has type FormData.validated FormData.formData
       but an expression was expected of type
         FormData.unvalidated FormData.formData
       Type FormData.validated is not compatible with type
         FormData.unvalidated
```

Check the [example](https://reasonml.github.io/en/try.html?reason=C4TwDgpgBA9mwEsYDsAUByAhgSigXgCgooAfKAZRgFsIMcjSoA5FCAbgIKpgBMBXADbRQkKADEYAJyoARTMEz4oAbwYjoAMymz5mOtg7F1UAG6YBCHvIg9DUY32RmLV4DbtDgULdLkKAXFAAzsCSCMgA5vgAfN7afnqOzpbWPAYMnlB8YJCSAMKYQRCBPjoKqEnmKW5pMXG+uhVOVa426cSZya0l8Y2VLqm4eLFwiCiopQmoXYPpAL4cXLyC0BINAeK9CkqqRuCaW3pYQ8Gh4RF2xjM1l-tZzQM3GRBek7pKisNQmB4vWTkQfKFaB4b51chnSIAOmyuQAxsDUDhfl5riCwcMGMQEBooKgIWFoUJIsAABZI3CxADMuF2xHplBoFLsxDmUAgAiKKix9JYyHYPIWBCFBEymECazKmChb3KkoSMIe1TaSnluhlh1QACItDAAEaYSRa9Kiv56wKjJBoNUKDXrPQ26VotInR1QtHMzgAKSCUIEMAinoA9AAqLGOOHUGjILzAGCmQE4kCcemZOH4LFBADuCGAcNJeL1tKxZEZtBwdTdsMBBSKFJLzFYdUwWJFUB9foDqDhBigIaDBCAA)

So we can't pass in a validated input into `upperCase`, but calling the same function with an unvalidated input will work.
We can already see the benefits.
This is all interesting but what if somebody wants to by pass the validation and by simply creating a validated type?

```reason
let byPassValidation: string => formData(validated) = a => a;
```

Interestingly the library user is free to do so.

```reason
let c = byPassValidation("ok!");
```

You might have noticed that we need to make an important change in our interface definition.
If you recall, we defined `formData` as:

```reason
type formData('a) = string;
```

But what we actually want to do is hide the implementation, to prevent a library user to arbitrarily bypass any internals, that we're actually trying to hide away.

By simply changing the type definition to:

```reason
type formData('a);
```

we can guarantee that our library functions as expected. The previously defined function:

```reason
 let byPassValidation: string => formData(validated) = a => a;
```

will not work anymore. The compiler will display an error:

```
Error: This expression has type string but an expression was expected of type
         FormData.validated FormData.formData
```

What happens when try validate our previously validated input?

```reason
let c = validate(b);
```

Again we receive the appropriate error message:

```
Error: This expression has type
         FormData.validated FormData.formData FormData.option
       but an expression was expected of type
         FormData.unvalidated FormData.formData

```

Check the [example](https://reasonml.github.io/en/try.html?reason=C4TwDgpgBA9mwEsYDsAUByAhgSigXgCgooAfKAZRgFsIMcjSoA5FCAbgIKpgBMBXADbRQkKADEYAJyoARTMEz4oAbwYjoAMymz5mOtg7F1UAG6YBCHvIg9DUY32RmLV4DbtDgULdLkKAXFAAzsCSCMgA5vgAfN7afnqOzpbWPAYMnlB8YJCSAMKYQRCBPjoKqEnmKW5pMXG+uhVOVa426cSZya0l8Y2VLqm4eLFwiCiopQmoXYPpAL4cXLyC0BINAeK9CkqqRuCaW3pYQ8Gh4RF2xjM1l-tZzQM3GRBek7pKisNQmB4vWTkQfKFaB4b51chnSIAOmyuQAxsDUDhfl5riCwcMGMQEBooKgIWFoUJIsAABZI3CxADMuF2xHplBoFLsxDmUAgAiKKix9JYyHYPIWBCFBEymECazKmChb3KkoSMIe1TaSnluhlh1QACItDAAEaYSRa9IAegAVERHHDqDRkF5gDBTICcSBOFBMnqQAAFQpBABqLXkSGQgRChKiXzVCg16z0UelaNqoM+sR+RA9Sk9PqC-sDYzQWpgAGsAITGxZQABSQShAhgEVQevS6b+cNVhxhAKBRUbzarNbrDbh7TNJoIQA)

To wrap this all up, here is how our library might be implemented in the real world:


```reason
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
```

Check the [example](https://reasonml.github.io/en/try.html?reason=C4TwDgpgBAbghgGwJYBM7Aig3AKB6SKAVwDt5k0Ns8BbAexSIWgOgDE6AnGgEXTigBeKAG8cUKK0kAKAORwAlLgnNgUAMacI6CAC4oAZ2CckJAOZCAfDNLlUOlEvFRVsRPYz7g02+8qYFKxk7f0dlFwg1IjBITgBhOAM9GXlAwWtvVPDXYDoAZWNTMy85RSCjE3NcAF9cHHpGZigObj5gOH0W3n4hUWcpTLLhCqLsyLcKHV6BdKg4MbVNbQxpoPnnV2jYhKTV2YLKswA6LYhOdUSIaUUFyXzC8z3rddq8AHoAKigIAA84GjATQ+bxwrhmzS43XaRyWOmkACIAGZ0OgAIzgnHRAC94U5QeNUb0um04EcQnCbngAFIGI4IOhmaSovE0ukMhEAWi53J5vL5-NxdVZ9MZxP4JxiZx2V0ULNpIs5-KVyu5gup8vZYuhp3ilyZCjlbMZ8JVpoFTk+UAA7kgEAgoFoDEw1HASN9OJwuFBgThLcLNZCSWS-HDmUpvSCgA)

### Summary
With phantom types we can enforce specific types to user land without exposing how these can be constructed.
This opens up a number of possible opportunities we can leverage, i.e. guaranteeing a value is `> 100` throughout our application.

If you have feedback, insights or question: [@twitter](https://twitter.com/sharifsbeat)
