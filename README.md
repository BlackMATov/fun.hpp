# fun.hpp

[![travis][badge.travis]][travis]
[![appveyor][badge.appveyor]][appveyor]
[![codecov][badge.codecov]][codecov]
[![language][badge.language]][language]
[![license][badge.license]][license]
[![paypal][badge.paypal]][paypal]

[badge.travis]: https://img.shields.io/travis/BlackMATov/fun.hpp/master.svg?logo=travis
[badge.appveyor]: https://img.shields.io/appveyor/ci/BlackMATov/fun-hpp/master.svg?logo=appveyor
[badge.codecov]: https://img.shields.io/codecov/c/github/BlackMATov/fun.hpp/master.svg?logo=codecov
[badge.language]: https://img.shields.io/badge/language-C%2B%2B17-red.svg
[badge.license]: https://img.shields.io/badge/license-MIT-blue.svg
[badge.paypal]: https://img.shields.io/badge/donate-PayPal-orange.svg?logo=paypal&colorA=00457C

[travis]: https://travis-ci.org/BlackMATov/fun.hpp
[appveyor]: https://ci.appveyor.com/project/BlackMATov/fun-hpp
[codecov]: https://codecov.io/gh/BlackMATov/fun.hpp
[language]: https://en.wikipedia.org/wiki/C%2B%2B17
[license]: https://en.wikipedia.org/wiki/MIT_License
[paypal]: https://www.paypal.me/matov

[fun]: https://github.com/BlackMATov/fun.hpp

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

### Any/All

```cpp
#include "fun/types/any.hpp"
#include "fun/types/all.hpp"
using namespace fun;

any_t a = make_any(true);
REQUIRE(a.get_any());

all_t b = make_all(false);
REQUIRE_FALSE(b.get_all());
```

### Sum/Product

```cpp
#include "fun/types/sum.hpp"
#include "fun/types/product.hpp"
using namespace fun;

sum_t<int> a = make_sum(10);
REQUIRE(a.get_sum() == 10);

product_t<int> b = make_product(20);
REQUIRE(b.get_product() == 20);
```

## Type Classes

### Functor

```c++
#include <fun/classes/functor.hpp>
#include <fun/instances/functor.hpp>

using namespace fun;
using namespace fun::underscore;
using namespace fun::functor_ops;

maybe_t<int> m = make_just(21);
maybe_t<unsigned> m1 = functor_f::fmap(_ * 2u, m);
REQUIRE(*m1 == 42);

maybe_t<int> n = make_nothing<int>();
maybe_t<float> n1 = (_ * 2.f) >>= n; // fmap operator syntax
REQUIRE(n1.is_nothing());
```

### Applicative

```cpp
#include <fun/classes/applicative.hpp>
#include <fun/instances/applicative.hpp>

using namespace fun;
using namespace fun::underscore;
using namespace fun::applicative_ops;

maybe_t<int> m = applicative_f::apply(make_just(_+20), make_just(22));
REQUIRE(*m == 42);

maybe_t<int> n = make_nothing<int>();
maybe_t<int> n1 = make_just(_+20) >>= n; // apply operator syntax
REQUIRE(n1.is_nothing());
```

### Alternative

```cpp
#include "fun/classes/alternative.hpp"
#include "fun/instances/alternative.hpp"

using namespace fun;
using namespace fun::alternative_ops;

maybe_t<int> m = alternative_f::alter(make_nothing<int>(), make_just<int>(42));
REQUIRE(*m == 42);

maybe_t<int> n = alternative_f::empty<maybe_t, int>();
maybe_t<int> n1 = n || make_just(42); // alter operator syntax
REQUIRE(*n1 == 42);
```

### Monoid

```cpp
#include <fun/classes/monoid.hpp>
#include <fun/instances/monoid.hpp>

using namespace fun;
using namespace fun::monoid_ops;

REQUIRE(monoid_f::append(make_any(true), make_any(false)).get_any());
REQUIRE(monoid_f::append(make_all(true), make_all(true)).get_all());
REQUIRE_FALSE(monoid_f::append(make_all(true), make_all(false)).get_all());

// and for Maybe of Monoid types of course
REQUIRE((
    make_just(make_sum(32)) >>=
    make_nothing<sum_t<int>>() >>=
    make_just(make_sum(10))
)->get_sum() == 42);
```

### Foldable

```cpp
#include <fun/classes/foldable.hpp>
#include <fun/instances/foldable.hpp>

using namespace fun;
using namespace fun::eq_ops;
using namespace fun::underscore;

REQUIRE((foldable_f::fold(fjust(make_sum(10))) == fsum(10)));
REQUIRE((foldable_f::fold(fnothing<sum_t<int>>) == fsum(0)));

REQUIRE((foldable_f::fold_map(fsum * (_*2), fjust(10)) == fsum(20)));
REQUIRE((foldable_f::fold_map(fsum * (_*2), fnothing<int>) == fsum(0)));
```

### Monad

```cpp
#include <fun/classes/eq.hpp>
#include <fun/classes/monad.hpp>
#include <fun/instances/monad.hpp>

using namespace fun;
using namespace fun::eq_ops;
using namespace fun::monad_ops;
using namespace fun::underscore;

REQUIRE((monad_f::mreturn<maybe_t>(10) == make_just(10)));
REQUIRE((monad_f::mbind(fjust(10), fjust * (_*2)) == fjust(20)));
REQUIRE((monad_f::mbind_const(fjust(10), fjust(20)) == make_just(20)));

// and operators for Haskell maniacs :)
REQUIRE(((fjust(10) >> fjust(20)) == fjust(20)));
REQUIRE(((fjust(10) >>= fjust * (_*2)) == fjust(20)));
```

#### Do notation :warning::see_no_evil::smiling_imp:

```cpp
#include <fun/classes/eq.hpp>
#include <fun/classes/monad.hpp>
#include <fun/instances/monad.hpp>

using namespace fun;
using namespace fun::eq_ops;

auto m = mdo<maybe_t>(
    []()               { return fjust(32); },
    [](int x1)         { (void)x1; return fjust(10); },
    [](int x1, int x2) { return mreturn<maybe_t>(x1 + x2); }
);
REQUIRE(m == make_just(42));

auto m2 = monad_f::mdo<maybe_t>(
    fjust(32),
    fnothing<int>,
    []() { return monad_f::mreturn<maybe_t>(42); }
);
REQUIRE((m2 == make_nothing<int>()));
```

## [License (MIT)](./LICENSE.md)