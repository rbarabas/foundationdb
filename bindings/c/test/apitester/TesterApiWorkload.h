/*
 * TesterApiWorkload.h
 *
 * This source file is part of the FoundationDB open source project
 *
 * Copyright 2013-2022 Apple Inc. and the FoundationDB project authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef APITESTER_API_WORKLOAD_H
#define APITESTER_API_WORKLOAD_H

#include "TesterWorkload.h"
#include "TesterKeyValueStore.h"
#include <atomic>

namespace FdbApiTester {

/**
 * Base class for implementing API testing workloads.
 * Provides various helper methods and reusable configuration parameters
 */
class ApiWorkload : public WorkloadBase, IWorkloadControlIfc {
public:
	void start() override;

	IWorkloadControlIfc* getControlIfc() override;

	virtual void stop() override;

	virtual void checkProgress() override;

	// Running specific tests
	// The default implementation generates a workload consisting of
	// random operations generated by randomOperation
	virtual void runTests();

	// Generate a random operation and schedule the continuation when done
	virtual void randomOperation(TTaskFct cont);

protected:
	// Selected FDB API version
	int apiVersion;

	// The minimum length of a key
	int minKeyLength;

	// The maximum length of a key
	int maxKeyLength;

	// The minimum length of a value
	int minValueLength;

	// The maximum length of a value
	int maxValueLength;

	// Maximum number of keys to be accessed by a transaction
	int maxKeysPerTransaction;

	// Initial data size (number of key-value pairs)
	int initialSize;

	// The ratio of reading existing keys
	double readExistingKeysRatio;

	// Run the workload until explicit stop
	bool runUntilStop;

	// The number of operations to be executed (for runUntilStop=false)
	int numRandomOperations;

	// The number of transactions to be completed for
	// a successful test progress check
	int numOperationsForProgressCheck;

	// Stop command received (for runUntilStop=true)
	std::atomic<bool> stopReceived;

	// Progress check is active (for runUntilStop=true)
	std::atomic<bool> checkingProgress;

	// Number of random operations left (for runUntilStop=false)
	std::atomic<int> numRandomOpLeft;

	// Key prefix
	std::string keyPrefix;

	// In-memory store maintaining expected database state
	KeyValueStore store;

	ApiWorkload(const WorkloadConfig& config);

	// Methods for generating random keys and values
	std::string randomKeyName();
	std::string randomValue();
	std::string randomNotExistingKey();
	std::string randomExistingKey();
	std::string randomKey(double existingKeyRatio);

	// Generate initial random data for the workload
	void populateData(TTaskFct cont);

	// Clear the data of the workload
	void clearData(TTaskFct cont);

	// common operations
	void randomInsertOp(TTaskFct cont);
	void randomClearOp(TTaskFct cont);
	void randomClearRangeOp(TTaskFct cont);

private:
	void populateDataTx(TTaskFct cont);

	void randomOperations();
};

} // namespace FdbApiTester

#endif