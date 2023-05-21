import { Api, StackContext } from "sst/constructs";

export function ApiStack({ stack }: StackContext) {
  // Create the HTTP API
  const api = new Api(stack, "Api", {
    routes: {
      "POST /send": "packages/functions/src/send.handler",
      "GET /update": "packages/functions/src/update.handler",
    },
  });

  // Show the API endpoint in the output
  stack.addOutputs({
    ApiEndpoint: api.url,
  });
}
