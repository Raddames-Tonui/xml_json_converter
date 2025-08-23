| Enum Value                 | Code | Meaning                                     |
| -------------------------- | ---- | ------------------------------------------- |
| `k200OK`                   | 200  | Success (default if you don’t set)          |
| `k201Created`              | 201  | Resource created                            |
| `k202Accepted`             | 202  | Request accepted, processing later          |
| `k204NoContent`            | 204  | Success, but no body                        |
| `k301MovedPermanently`     | 301  | Permanent redirect                          |
| `k302Found`                | 302  | Temporary redirect                          |
| `k304NotModified`          | 304  | Resource not changed (cache)                |
| `k400BadRequest`           | 400  | Invalid request (bad JSON, params, etc.)    |
| `k401Unauthorized`         | 401  | Authentication required                     |
| `k403Forbidden`            | 403  | Permission denied                           |
| `k404NotFound`             | 404  | Resource not found                          |
| `k405MethodNotAllowed`     | 405  | Wrong HTTP method                           |
| `k409Conflict`             | 409  | Resource conflict                           |
| `k415UnsupportedMediaType` | 415  | Wrong content type                          |
| `k429TooManyRequests`      | 429  | Rate limited                                |
| `k500InternalServerError`  | 500  | Server crashed internally                   |
| `k502BadGateway`           | 502  | Bad proxy/gateway                           |
| `k503ServiceUnavailable`   | 503  | Service unavailable (maintenance, overload) |
| `k504GatewayTimeout`       | 504  | Gateway timeout                             |
