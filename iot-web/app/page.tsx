"use client";

import { useEffect, useState } from "react";
import { Record } from "./lib/records";
import { Spinner } from "@nextui-org/react";

export default function Page() {
  const [lastRecords, setLastRecords] = useState<Record[]>([]);
  const [loading, setLoading] = useState<boolean>(true);

  useEffect(() => {
    fetch("https://kn8k5rd1-3000.usw3.devtunnels.ms/api/getReadings")
      .then((response) => response.json())
      .then((data) => {
        setLastRecords(data.records); // Set records directly
        setLoading(false);
      })
      .catch((error) => {
        console.error("Error fetching records:", error);
        setLoading(false);
      });
  }, []);

  return (
    <main className="text-white">
      <h1 className="text-center my-10 text-3xl">Lecturas del ultrasónico</h1>
      {loading ? (
        <Spinner className="flex self-center" />
      ) : lastRecords.length > 0 ? (
        <div className="overflow-x-auto mx-10">
          <table className="w-full text-white border-collapse">
            <thead>
              <tr className="border-b border-gray-700">
                <th className="p-2">ID</th>
                <th className="p-2">Device ID</th>
                <th className="p-2">Distance</th>
                <th className="p-2">Date</th>
              </tr>
            </thead>
            <tbody>
              {lastRecords.map((record) => (
                <tr key={record.id} className="border-b border-gray-800">
                  <td className="p-2 text-center">{record.id}</td>
                  <td className="p-2 text-center">{record.deviceId}</td>
                  <td className="p-2 text-center">{record.distance}</td>
                  <td className="p-2 text-center">
                    {new Date(record.createdAt).toLocaleDateString()}
                  </td>
                </tr>
              ))}
            </tbody>
          </table>
        </div>
      ) : (
        <p className="text-center mt-4">No records found</p>
      )}
    </main>
  );
}
