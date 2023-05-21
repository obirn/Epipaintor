import notes from "@rest-api/core/notes";
import { APIGatewayProxyHandlerV2 } from "aws-lambda";

export const handler: APIGatewayProxyHandlerV2 = async (event) => {
  console.log("event body = ");
  console.log(event.body);
  return {
    statusCode: 200,
    body: "The image has been updated !",
  };
}
