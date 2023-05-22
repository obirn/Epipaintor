import notes from "@rest-api/core/notes";
import { GetObjectCommand, S3Client } from '@aws-sdk/client-s3';
import { APIGatewayProxyHandlerV2 } from "aws-lambda";

const s3Client = new S3Client({});

const streamToString = (stream: any): Promise<string> =>
  new Promise((resolve, reject) => {
    const chunks: any = [];
    stream.on('data', (chunk: any) => chunks.push(chunk));
    stream.on('error', reject);
    stream.on('end', () => resolve(Buffer.concat(chunks).toString('utf8')));
  });

export const handler: APIGatewayProxyHandlerV2 = async (event) => {
  console.log("event body = ");
  console.log(event.body);

  if (!event.body) return {
    statusCode: 400,
    body: "Missing key argument in body.",
  };

  console.log(event.body)
  const key = event.body;

  const params = {
    Bucket: process.env.BUCKET_NAME,
    Key: key,
  };

  const command = new GetObjectCommand(params);
  const response = await s3Client.send(command);

  const { Body } = response;

  const image = await streamToString(Body);
  return {
    statusCode: 200,
    body: image,
  };
}
