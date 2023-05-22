import { PutObjectCommand, S3Client } from '@aws-sdk/client-s3';
import { APIGatewayProxyHandlerV2 } from "aws-lambda";

const s3Client = new S3Client({});

export const handler: APIGatewayProxyHandlerV2 = async (event) => {
  console.log("event body for send.ts = ");
  console.log(event.body);
  try {
    const data = await s3Client.send(
      new PutObjectCommand({
        Bucket: process.env.BUCKET_NAME,
        Key: `epipaintor`,
        Body: JSON.stringify(event.body),
      }),
    );
    // console.log(data);
  } catch (err) {
    console.log('Put Error', err);
  }

  return {
    statusCode: 200,
    body: "The image has been saved !",
  };
}
