import { PutObjectCommand, S3Client } from '@aws-sdk/client-s3';
import { APIGatewayProxyHandlerV2 } from "aws-lambda";
import { getSignedUrl } from '@aws-sdk/s3-request-presigner';

const s3Client = new S3Client({});

export const handler: APIGatewayProxyHandlerV2 = async (event) => {
  console.log("event body for send.ts = ");
  console.log(event.body);
  const command = new PutObjectCommand({
    ACL: "public-read",
    Key: "epipaintor",
    Bucket: process.env.BUCKET_NAME,
  });
  const url = await getSignedUrl(new S3Client({}), command);

  return {
    statusCode: 200,
    body: url,
  };
}
