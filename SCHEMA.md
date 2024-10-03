# Types
## Primary
- number
- function(T: T)
- enum([K,Tk])
- map(K->Tk)
- char

## Derived
- boolean enum(True, False)
- string [char]


# Examples
```
import utils
import graphics.vk

map [ a -> number, b -> number, c -> number ] = [
  a -> 4,
  b -> 5,
  c -> 9
]

Result = T : E : enum(
  Success(T),
  Error(E)
)

DivisionResult = Result number string

divBy0 = DivisionResult.Error("Divided By 0")

arr [ _0 -> number, _1 -> number, _2 -> number ] = [ 1,2,3 ]


arr [number] = [2, 3, 4, 5]

map = Map.setDefaultFn(map, mp: {
  mp.a + mp.b
})

pub facto = x : if (x==0) (1) (x * facto(x-1))

if (T : (std.boolean : T : T : T))

main = args [string] : IO (
  print "Hello World",
  x <- std.parseIntIO(read()),
  print "Factorial of {x} is {facto x}"
)

```