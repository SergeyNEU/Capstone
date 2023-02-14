For this project, we will be using Amazon Web Services (AWS) to host and deploy our data via the cloud from our mobile app and to our website. This README acts as a guide for us to access, create, manage, and manipulate our account for The Pothole Protector project. 

**Background and Services Info**

Our AWS account is under the Free Tier, which means we have 12 months of limited access to certain AWS services. We will be billed if we utilize services outside those listed in the free tier. Further, if we use more than the certain limits to the services covered under the free tier, we will also be billed.

The AWS Free Tier comes with many features, which can be seen [here](https://aws.amazon.com/free/?all-free-tier.sort-by=item.additionalFields.SortRank&all-free-tier.sort-order=asc&awsf.Free%20Tier%20Types=*all&awsf.Free%20Tier%20Categories=*all).  We also have access to training material through the AWS Educate program. Both were provided through the school.

For this project, we will primarily be using [S3 Buckets](https://aws.amazon.com/s3/), which stands for Simple Storage System. We get 5GB of storage, 20k Get Requests, and 2k Put requests through the Free Tier. We organize the data in S3 Object Tags, and can use this to send out data later. We may also utilize both the [AWS Lambda](https://aws.amazon.com/lambda/) and [AWS Integration](https://aws.amazon.com/free/application-integration/) services for a variety of usecases. AWS Lambda is used for event-driven code execution, which can be used to populate the map by retrieving the location/photo/accelerometer information in the S3 bucket. We may use AWS integration services such as AWS API Gateway to make a REST API call to an endpoint, AWS SNS for mobile notifications of a pothole, and Amazon Cognito to create accounts.

**Access**

I am the root, or primary, user for our project account. The email for this account is under my Northeastern email, kolli.na@northeastern.edu - If you would like access, reach out to me, and I will add you to the Access Control List (IAM user). username: kolli.na@northeastern.edu ; password: Sn1taXkCPBT07V4!@

**AWS Links**

1. [Uploading to S3 from Mobile App](https://aws.amazon.com/blogs/compute/uploading-to-amazon-s3-directly-from-a-web-or-mobile-application/)
2. [Amazon Rekognition](https://aws.amazon.com/rekognition/?did=ft_card&trk=ft_card) - We have access to analyze 5,000 images per month through the free tier. AWS Recognition is image recognition software that can perform object detection and can create labels with confidence score.
