import { Api, StackContext, Function, Bucket } from "sst/constructs";
// import * as sst from "@serverless-stack/resources";
// import * as s3 from "@aws-cdk/aws-s3";
// import * as lambda from "@aws-cdk/aws-lambda";


export function ApiStack({ stack }: StackContext) {

  const bucket = new Bucket(stack, "Bucket");

  // Create the HTTP API
  const api = new Api(stack, "Api", {
    defaults: {
      function: {
        environment: { BUCKET_NAME: bucket.bucketName, },
        permissions: [bucket],
      },
    },
    routes: {
      "POST /send": "packages/functions/src/send.handler",
      "GET /url": "packages/functions/src/url.handler",
    },
  });

  // Show the API endpoint in the output
  stack.addOutputs({
    ApiEndpoint: api.url,
  });
}
