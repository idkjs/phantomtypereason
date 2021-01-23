Functional Domain Modelling Workshop - new
==========================================

fetched from https://sketch.sh/s/sbhQQPx64lXrk3hEwirKIS

# Basic Building Blocks
### Primitives
```reason
/* primitives (type alias) */
type name = string
type age = int
type size = float
type isAlive = bool


let a: int = 27
let a: age = 27
let a = 27
let a = 5.7;

27;

"foo";
```
### Abstract Types
important usage:
 1. reduce implementation detail
 2. model top-down
```reason
type notYetSpecified;
```
# Algebraic Data Types (ADT)
### Sum Types (OR) 
(also Variants, Unions)
```reason
type color = Red | Green | Blue | Yellow;
type shape = Circle | Rectangle;

/* 4 possible Values (sum) */
Red;
```
### Product Types (AND)
```reason
/* Product: Tuple */
type coordinate = (int, int);
type coloredShape = (color, shape);

/* 4 Colors * 2 Shapes = 8 possible Values (product) */
(Red, Circle);
(7, 3)

/* Product: Record */
type coloredShape = {shape, color};

{shape: Circle, color: Red};
{color: Green, shape: Rectangle};
```
### Sum Type with Payload
```reason
/* Sum with Payload aka Type Constructors */
type shape = Circle(int) | Square(int, int);

Square(3, 4); /* (3) */
Circle(4); /* (3, 4) */
```
### Primitive Anti-Pattern
```reason
let durationToNextRoom = 10.0;
let wayToNextRoom = 5.0;

let speed = (way: float, time: float) => way /. time;

speed(durationToNextRoom, wayToNextRoom);
```
### Tagged Types (Sum Type of 1)
```reason
type secs = | Secs(float);
type meter = | Meter(float);

let speed = (Meter(m), Secs(s)) => m /. s;

speed(Meter(5.0), Secs(10.0));
```
# Lists
```reason
let x: list(shape) = [];

type simplePicture = { shape: list(shape),  backgroundColor: color};
```
# Functions
```reason
type areaSize = shape => size
type add = (float, float) => float;
```
# Invariant Enforcement

Tennis Scoring Rules:
* Player points: Love, 15, 30, 40
* 40 points && win the ball ⇒ win game
* both player 40 ⇒ players are deuce
* deuce: the winner of a ball ⇒ advantage
* advantage && wins the ball ⇒  win game
* player without advantage wins ⇒ back at deuce
## 1st Try
```reason
type points = int;

type score = {
  playerOne: points,
  playerTwo: points,
};

let s = {
  playerOne: 1000, /* 1000, -200 */
  playerTwo: -15,
};
```
## 2nd Try
```reason
type points =
  | Love
  | Fifteen
  | Thirty
  | Forty;

type score = {
  playerOne: points,
  playerTwo: points,
};

let s2 = {playerOne: Fifteen, playerTwo: Love};
let even = {playerOne: Forty, playerTwo: Forty};
```
## Better Solution
```reason
type player =
  | PlayerOne
  | PlayerTwo;

type points =
  | Love
  | Fifteen
  | Thirty;

type score =
  | Points(points, points)
  | Forty(player, points /* of other player */)
  | Deuce
  | Advantage(player)
  | Game(player);

let startScore: score = Points(Love, Love);
let anotherScore: score = Forty(PlayerTwo, Thirty);
let anotherScore2: score = Deuce;
let anotherScore3: score = Advantage(PlayerOne);

/* 
 let impossibleScore1: score = Points(Seven, Eleven);
 let impossibleScore2: score = Points(Forty, Forty);
 let impossibleScore3: score = Forty(PlayerTwo, Forty);
 */
```
# Optional, ie. Billion Dollar Mistake
```reason
/* type option('a) = None | Some('a); */

let reciprocal = x => x == 0.0 ? None : Some(1.0 /. x);

type name = Name(string);
type phone = Phone(string);

type customer = {
  name,
  phone: option(phone),
};
```
# Tactical Design Pattern and idioms
## Model Values explicitly!
ie. replace booleans with variants
```reason
/* Don't */

let cellIsAlive: bool = true;

type livingState =
  | ALIVE
  | DEAD;
/*  Do */
let cell = ALIVE;
```
## Smart Constructors
https://github.com/ostera/reason-design-patterns/blob/master/patterns/smart-constructors.md
```reason
type name = Name(string); /* Business Contraint: Length < 20 */

type makeName = string => option(name);
let makeName: makeName = str => (str |> String.length < 20) ? Some(Name(str)) : None

makeName("Sven");

makeName("Foo bar Foo bar Foo bar ");
```
## Model State Changes explicitly!
 * Move Runtime to Compile Time
 * Bool -> Type
```reason
/* Bad Idea */
type emailAdress = {
  email: string,
  verified: bool,
};

/* Better */
type unverifiedEmail =
  | UnverifiedEmailAdress(string);
type verifiedEmail =
  | VerifiedEmailAdress(string);

type email =
  | UnverifiedEmailAdress(string)
  | VerifiedEmailAdress(string);

type verifyEmail = unverifiedEmail => option(verifiedEmail);
```
## Model with Abstract Types;
```reason
type address;

type customer = {
  name,
  address,
};
```
## Generics
```reason
type numberOfPlayers = | NumberOfPlayers(int);
type duration = Minutes(int) | Hours(int);
type boardgame = {numberOfPlayers: int, duration: duration};

type cover = Leather | Cardboard;
type notebooks = {numberOfPages: int, cover: cover};

type date;
type id;

type product('productKind) = {  
  productId: id,
  inShopSince: option(date),
  productSpecificData: 'productKind,
};

let productIdOf = product => product.productId;

let numberOfPlayersOf = product => product.productSpecificData.numberOfPlayers;
```
