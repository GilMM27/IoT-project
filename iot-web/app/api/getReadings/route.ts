import { NextResponse } from "next/server";
import { PrismaClient } from "@prisma/client";

const prisma = new PrismaClient();

export async function GET(request: Request) {
  try {
    const rawRecords = await prisma.movementRecord.findMany({
      orderBy: { createdAt: "desc" },
      include: {
        device: {
          select: {
            name: true
          }
        }
      },
      take: 10,
    });

    const records = rawRecords.map(record => ({
      id: record.id,
      distance: record.distance,
      deviceId: record.deviceId,
      createdAt: record.createdAt,
      deviceName: record.device.name
    }));

    return NextResponse.json({ records }, { status: 200 });
  } catch (error) {
    console.error("Error fetching records:", error); // Added for debugging
    return NextResponse.json(
      { error: "Failed to fetch records" },
      { status: 500 }
    );
  }
}
