import notes from "@rest-api/core/notes";
import { GetObjectCommand, S3Client } from '@aws-sdk/client-s3';
import { getSignedUrl } from '@aws-sdk/s3-request-presigner';
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

  if (!event.queryStringParameters || !event.queryStringParameters.key) return {
    statusCode: 400,
    body: "Missing key argument in queryStringParameters.",
  };

  const key = event.queryStringParameters.key;

  const params = {
    Bucket: process.env.BUCKET_NAME,
    Key: key,
  };

  
  try {
    console.log(key);
    
    const command = new GetObjectCommand(params);
    const expiriesInTime = 60 * 60;
    const presignedUrl = await getSignedUrl(s3Client, command, { expiresIn: expiriesInTime }); // URL valid for 1 hour

    return {
      statusCode: 200,
      body: JSON.stringify({ url: presignedUrl }),
    };
  } catch {
    return {
      statusCode: 200,
      body: "No bucket found.",
    };
  }
}
