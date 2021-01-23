Phantom Types in ReasonML
=========================

fetched from https://sketch.sh/s/nerP3hkOxX6sMVvVUtBIbs


### Introduction

*"A phantom type is a parametrised type whose parameters do not all appear on the right-hand side of its definition..."*
[Haskell Wiki, PhantomType](https://wiki.haskell.org/Phantom_type)

The following write-up is intended as an introduction into using phantom types in **ReasonML**.

Taking a look at the above definition from the Haskell wiki, it states that phantom types are parametrised types where not all parameters appear on the right-hand side. Let's try to see if we can implement a similar example as in said wiki.


### Example
```reason
type formData('a) = string;
```

So `formData` is a phantom type as the `'a` parameter only appears on the left side.

Let's create a file `FormData.rei` to define the `formData` interface and add the above definition.

We want to enable a library user to create a `formData` type. What we also want is to restrict the type in certain parts of the library. For example we want to be able to differentiate between *validated* und *unvalidated* form data. So let's add two type definitions `validated` and `unvalidated` to our `FormData.rei` file.
```reason
module type FormData = {
  type formData('a);
  type validated;
  type unvalidated;
 };
```
Nothing special up until here, just two lone type definitions.
Next we would want to expose a function that receives a string and returns an unvalidated `formData` type, which we will do by extending our API.
```reason
module type FormData = {
  type formData('a);
  type validated;
  type unvalidated;
  let formData: string => formData(unvalidated);
};
```
And maybe we want to add an `upperCase` function that does exactly that, take a unvalidated input and return an unvalidated input, so again let's update our API.
```reason
module type FormData = {
  type formData('a);
  type validated;
  type unvalidated;
  let formData: string => formData(unvalidated);
  let upperCase: formData(unvalidated) => formData(unvalidated);
};
```
Finally let's also add a `validate` function that either returns nothing or the validated input.
```reason
type option('a) =
  | Some('a)
  | None;

module type FormData = {
  type formData('a);
  type validated;
  type unvalidated;
  let formData: string => formData(unvalidated);
  let upperCase: formData(unvalidated) => formData(unvalidated);
  let validate: formData(unvalidated) => option(formData(validated));
};

```
Next, we'll create a new file and name it `FormData.re`, where we will implement the interface.
```reason
module FormData : FormData = {
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
};
```
Let's try to use the library:
```reason
open FormData;

let initialData: formData(unvalidated) = FormData.formData("test");
let validatedData: option(formData(validated)) = validate(initialData);

```
So we can see that this works as expected. We validate "test" and get the validated form input in return. But what if we wanted to call the `upperCase` function?
```reason
/* Uncomment to see the error! */
/*switch (validatedData) {
| Some(result) => FormData.upperCase(result)
| None => a
};*/
```
The compiler will complain:

```
Error: This expression has type FormData.formData(FormData.validated) 
       but an expression was expected of type
         FormData.formData(FormData.unvalidated)
       Type FormData.validated is not compatible with type
         FormData.unvalidated
```


So we can't pass in a validated input into `upperCase`, but calling the same function with an unvalidated input will work. We can already see the benefits.
```reason
FormData.upperCase(initialData);
```
This is all interesting but what if somebody wants to bypass the validation and by simply creating a *validated* type?
```reason
/* Uncomment to see the error! */
/* 
let byPassValidation: string => formData(validated) = a => a;
*/
```
The compiler will display an error:

```
Error: This expression has type string but an expression was expected of type
         FormData.formData(FormData.validated)
```
If you recall we defined the type `formData` inside the interface as follows:

```reason
type formData('a);
```

By taking this approach, we're hiding the implementation, to prevent a library user to arbitrarily bypass any internals. This important. Because we chose to define the `formData` type inside the interface definition user land could simply bypass the validation.

```reason
let byPassValidation: string => formData(validated) = a => a;
```

The above example would work, if you would expose the type definition inside the interface.


Finally, what would happen when we try validate our previously validated input?
```reason
/* Uncomment to see the error! */
/*switch (validatedData) {
| Some(result) => FormData.upperCase(result)
| None => a
}*/
```
Again we receive the appropriate error message:

```
Error: This expression has type FormData.formData(FormData.validated)
       but an expression was expected of type
         FormData.formData(FormData.unvalidated)
       Type FormData.validated is not compatible with typw
       	 FormData.unvalidated
```

### Summary
With phantom types we can enforce specific types to user land without exposing how these can be constructed.
This opens up a number of possible opportunities we can leverage, i.e. guaranteeing a value is `> 100` throughout our application.

Check the original gist [here](https://gist.github.com/busypeoples/3a28d039272ec3eb33ca2fc6b32dafc7)

If you have feedback, insights or questions: [@twitter](https://twitter.com/sharifsbeat)
