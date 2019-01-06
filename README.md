# fun.hpp



## Types

### Box

```cpp
#include "fun/types/box.hpp"
using namespace fun;

box_t<int> b = make_box(42);
REQUIRE(*b == 42);
```

### Maybe

```cpp
#include "fun/types/maybe.hpp"
using namespace fun;

maybe_t<int> m = make_just(42);
REQUIRE(m.is_just());
REQUIRE(*m == 42);

maybe_t<int> n = make_nothing<int>();
REQUIRE(n.is_nothing());
REQUIRE_THROWS(*n);
```



## Type Classes

### Functor

```c++
#include <fun/classes/functor.hpp>
#include <fun/instances/functor.hpp>

using namespace fun;
using namespace fun::underscore;

maybe_t<int> m = make_just(21);
maybe_t<unsigned> m1 = functor_f::fmap(_ * 2u, m);
REQUIRE(*m1 == 42);

maybe_t<int> n = make_nothing<int>();
maybe_t<float> n1 = (_ * 2.f) >>= n; // fmap operator syntax
REQUIRE(n1.is_nothing());
```



## [License (MIT)](./LICENSE.md)