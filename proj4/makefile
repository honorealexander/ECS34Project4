CC = gcc
CXX = g++

INC_DIR = ./include
SRC_DIR = ./src
OBJ_DIR = ./obj
BIN_DIR = ./bin
$(shell mkdir -p $(BIN_DIR))
TEST_SRC_DIR = ./testsrc

CXXFLAGS = -std=c++17 -I$(INC_DIR) -Wall
LDFLAGS = -lgtest -lgtest_main -lpthread -lexpat -lgmock

all: directories runtests transplanner speedtest

test: runtests

runtests: $(BIN_DIR)/teststrutils $(BIN_DIR)/teststrdatasource $(BIN_DIR)/teststrdatasink $(BIN_DIR)/testfiledatass $(BIN_DIR)/testdsv $(BIN_DIR)/testxml $(BIN_DIR)/testcsvbs $(BIN_DIR)/testosm $(BIN_DIR)/testdpr $(BIN_DIR)/testcsvbsi $(BIN_DIR)/testtpcl $(BIN_DIR)/testtp $(BIN_DIR)/testkml
	@echo "Running all tests..."
	@./$(BIN_DIR)/teststrutils
	@./$(BIN_DIR)/teststrdatasource
	@./$(BIN_DIR)/teststrdatasink
	@./$(BIN_DIR)/testfiledatass
	@./$(BIN_DIR)/testdsv
	@./$(BIN_DIR)/testxml
	@./$(BIN_DIR)/testcsvbs
	@./$(BIN_DIR)/testosm
	@./$(BIN_DIR)/testdpr
	@./$(BIN_DIR)/testcsvbsi
	@./$(BIN_DIR)/testtpcl
	@./$(BIN_DIR)/testtp
	@./$(BIN_DIR)/testkml

# teststrutils
$(BIN_DIR)/teststrutils: $(OBJ_DIR)/StringUtils.o $(OBJ_DIR)/StringUtilsTest.o
	$(CXX) -o $@ $(CXXFLAGS) $(OBJ_DIR)/StringUtils.o $(OBJ_DIR)/StringUtilsTest.o $(LDFLAGS)
	@echo "success 1..."
	@echo ""

$(OBJ_DIR)/StringUtils.o: $(SRC_DIR)/StringUtils.cpp $(INC_DIR)/StringUtils.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(SRC_DIR)/StringUtils.cpp

$(OBJ_DIR)/StringUtilsTest.o: $(TEST_SRC_DIR)/StringUtilsTest.cpp $(INC_DIR)/StringUtils.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(TEST_SRC_DIR)/StringUtilsTest.cpp

# teststrdatasource
$(BIN_DIR)/teststrdatasource: $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringDataSourceTest.o
	$(CXX) -o $@ $(CXXFLAGS) $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringDataSourceTest.o $(LDFLAGS)
	@echo "success 2..."
	@echo ""

$(OBJ_DIR)/StringDataSource.o: $(SRC_DIR)/StringDataSource.cpp $(INC_DIR)/StringDataSource.h $(INC_DIR)/DataSource.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(SRC_DIR)/StringDataSource.cpp

$(OBJ_DIR)/StringDataSourceTest.o: $(TEST_SRC_DIR)/StringDataSourceTest.cpp $(INC_DIR)/StringDataSource.h $(INC_DIR)/DataSource.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(TEST_SRC_DIR)/StringDataSourceTest.cpp

# teststrdatasink
$(BIN_DIR)/teststrdatasink: $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/StringDataSinkTest.o
	$(CXX) -o $@ $(CXXFLAGS) $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/StringDataSinkTest.o $(LDFLAGS)
	@echo "successful 3..."
	@echo ""

$(OBJ_DIR)/StringDataSink.o: $(SRC_DIR)/StringDataSink.cpp $(INC_DIR)/StringDataSink.h $(INC_DIR)/DataSink.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(SRC_DIR)/StringDataSink.cpp

$(OBJ_DIR)/StringDataSinkTest.o: $(TEST_SRC_DIR)/StringDataSinkTest.cpp $(INC_DIR)/StringDataSink.h $(INC_DIR)/DataSink.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(TEST_SRC_DIR)/StringDataSinkTest.cpp

#testfiledatass
$(BIN_DIR)/testfiledatass: $(OBJ_DIR)/FileDataSource.o $(OBJ_DIR)/FileDataSink.o $(OBJ_DIR)/FileDataFactory.o $(OBJ_DIR)/FileDataSSTest.o
	$(CXX) -o $@ $(CXXFLAGS) $(OBJ_DIR)/FileDataSource.o $(OBJ_DIR)/FileDataSink.o $(OBJ_DIR)/FileDataFactory.o $(OBJ_DIR)/FileDataSSTest.o $(LDFLAGS)
	@echo "SUCCESS"
	@echo ""

$(OBJ_DIR)/FileDataSource.o: $(SRC_DIR)/FileDataSource.cpp $(INC_DIR)/FileDataSource.h $(INC_DIR)/DataSource.h
	@mkdir -p $(OBJ_DIR)
	$(CXX) -o $@ -c $(CXXFLAGS) $(SRC_DIR)/FileDataSource.cpp

$(OBJ_DIR)/FileDataSink.o: $(SRC_DIR)/FileDataSink.cpp $(INC_DIR)/FileDataSink.h $(INC_DIR)/DataSink.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(SRC_DIR)/FileDataSink.cpp

$(OBJ_DIR)/FileDataFactory.o: $(SRC_DIR)/FileDataFactory.cpp $(INC_DIR)/FileDataFactory.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(SRC_DIR)/FileDataFactory.cpp

$(OBJ_DIR)/FileDataSSTest.o: $(TEST_SRC_DIR)/FileDataSSTest.cpp $(INC_DIR)/FileDataSource.h $(INC_DIR)/FileDataSink.h $(INC_DIR)/FileDataFactory.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(TEST_SRC_DIR)/FileDataSSTest.cpp

# testdsv
$(BIN_DIR)/testdsv: $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/DSVWriter.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/DSVTest.o $(OBJ_DIR)/StringUtils.o
	$(CXX) -o $@ $^ $(LDFLAGS)
	@echo "success 4..."
	@echo ""

$(OBJ_DIR)/DSVReader.o: $(SRC_DIR)/DSVReader.cpp $(INC_DIR)/DSVReader.h
	$(CXX) -o $@ -c $(SRC_DIR)/DSVReader.cpp $(CXXFLAGS)

$(OBJ_DIR)/DSVWriter.o: $(SRC_DIR)/DSVWriter.cpp $(INC_DIR)/DSVWriter.h
	$(CXX) -o $@ -c $(SRC_DIR)/DSVWriter.cpp $(CXXFLAGS)

$(OBJ_DIR)/DSVTest.o: $(TEST_SRC_DIR)/DSVTest.cpp $(INC_DIR)/DSVReader.h $(INC_DIR)/DSVWriter.h
	$(CXX) -o $@ -c $(TEST_SRC_DIR)/DSVTest.cpp $(CXXFLAGS)

# testxml
$(BIN_DIR)/testxml: $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/XMLWriter.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/XMLTest.o
	$(CXX) -o $@ $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/XMLWriter.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/XMLTest.o $(LDFLAGS)
	@echo "success 5..."
	@echo ""

$(OBJ_DIR)/XMLReader.o: $(SRC_DIR)/XMLReader.cpp $(INC_DIR)/XMLReader.h
	$(CXX) -o $@ -c $(SRC_DIR)/XMLReader.cpp $(CXXFLAGS)

$(OBJ_DIR)/XMLWriter.o: $(SRC_DIR)/XMLWriter.cpp $(INC_DIR)/XMLWriter.h
	$(CXX) -o $@ -c $(SRC_DIR)/XMLWriter.cpp $(CXXFLAGS)

$(OBJ_DIR)/XMLTest.o: $(TEST_SRC_DIR)/XMLTest.cpp $(INC_DIR)/XMLReader.h $(INC_DIR)/XMLWriter.h
	$(CXX) -o $@ -c $(TEST_SRC_DIR)/XMLTest.cpp $(CXXFLAGS)

# testcsvbs
$(BIN_DIR)/testcsvbs: $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/CSVBusSystemTest.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringUtils.o
	$(CXX) -o $@ $(CXXFLAGS) $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/CSVBusSystemTest.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringUtils.o $(LDFLAGS)
	@echo "success 6..."
	@echo ""

$(OBJ_DIR)/CSVBusSystem.o: $(SRC_DIR)/CSVBusSystem.cpp $(INC_DIR)/CSVBusSystem.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(SRC_DIR)/CSVBusSystem.cpp

$(OBJ_DIR)/CSVBusSystemTest.o: $(TEST_SRC_DIR)/CSVBusSystemTest.cpp $(INC_DIR)/CSVBusSystem.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(TEST_SRC_DIR)/CSVBusSystemTest.cpp

# testosm
$(BIN_DIR)/testosm: $(OBJ_DIR)/OpenStreetMap.o $(OBJ_DIR)/OpenStreetMapTest.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringUtils.o $(OBJ_DIR)/XMLReader.o
	$(CXX) -o $@ $^ $(LDFLAGS)
	@echo "success 7..."
	@echo ""

$(OBJ_DIR)/OpenStreetMapTest.o: $(TEST_SRC_DIR)/OSMTest.cpp $(INC_DIR)/OpenStreetMap.h $(INC_DIR)/StringUtils.h $(INC_DIR)/DSVReader.h $(INC_DIR)/StringDataSource.h $(INC_DIR)/XMLReader.h
	@mkdir -p $(OBJ_DIR)/tests
	$(CXX) -o $@ -c $(CXXFLAGS) $<

# testdpr
$(BIN_DIR)/testdpr: $(OBJ_DIR)/CDijkstraPathRouter.o $(OBJ_DIR)/CDijkstraPathRouterTest.o
	@mkdir -p $(BIN_DIR)  #create bin directory
	$(CXX) -o $@ $^ $(LDFLAGS)
	@echo "success 8..."
	@echo ""

$(OBJ_DIR)/CDijkstraPathRouter.o: $(SRC_DIR)/CDijkstraPathRouter.cpp $(INC_DIR)/PathRouter.h $(INC_DIR)/DijkstraPathRouter.h 
	@mkdir -p $(OBJ_DIR)  # Add this line to create the obj directory
	$(CXX) -o $@ -c $(CXXFLAGS) $(SRC_DIR)/CDijkstraPathRouter.cpp

$(OBJ_DIR)/CDijkstraPathRouterTest.o: $(TEST_SRC_DIR)/CDijkstraPathRouterTest.cpp $(INC_DIR)/PathRouter.h $(INC_DIR)/DijkstraPathRouter.h 
	$(CXX) -o $@ -c $(CXXFLAGS) $(TEST_SRC_DIR)/CDijkstraPathRouterTest.cpp

# testcsvbsi
$(BIN_DIR)/testcsvbsi: $(OBJ_DIR)/CBusSystemIndexer.o $(OBJ_DIR)/CSVBusSystemIndexerTest.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringUtils.o
	$(CXX) -o $@ $^ $(LDFLAGS)
	@echo "success 9..."
	@echo ""

$(OBJ_DIR)/CBusSystemIndexer.o: $(SRC_DIR)/CBusSystemIndexer.cpp $(INC_DIR)/BusSystemIndexer.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(SRC_DIR)/CBusSystemIndexer.cpp

$(OBJ_DIR)/CSVBusSystemIndexerTest.o: $(TEST_SRC_DIR)/CSVBusSystemIndexerTest.cpp $(INC_DIR)/BusSystemIndexer.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(TEST_SRC_DIR)/CSVBusSystemIndexerTest.cpp

# testtpcl
$(BIN_DIR)/testtpcl: $(OBJ_DIR)/TransportationPlannerCommandLine.o $(OBJ_DIR)/TPCommandLineTest.o $(OBJ_DIR)/FileDataSource.o $(OBJ_DIR)/StandardDataSink.o $(OBJ_DIR)/StandardDataSource.o $(OBJ_DIR)/StandardErrorDataSink.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/OpenStreetMap.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/FileDataSink.o $(OBJ_DIR)/FileDataFactory.o $(OBJ_DIR)/FileDataSink.o $(OBJ_DIR)/StringUtils.o 
	$(CXX) -o $@ $^ $(LDFLAGS)
	@echo "success 10..."
	@echo ""

$(OBJ_DIR)/TransportationPlannerCommandLine.o: $(SRC_DIR)/CTransportationPlannerCommandL.cpp $(INC_DIR)/TransportationPlanner.h $(INC_DIR)/TransportationPlannerCommandLine.h $(INC_DIR)/TransportationPlannerConfig.h 
	@mkdir -p $(OBJ_DIR)
	$(CXX) -o $@ -c $(CXXFLAGS) $(SRC_DIR)/CTransportationPlannerCommandL.cpp

$(OBJ_DIR)/TPCommandLineTest.o: $(TEST_SRC_DIR)/TPCommandLineTest.cpp $(INC_DIR)/TransportationPlanner.h $(INC_DIR)/TransportationPlannerCommandLine.h $(INC_DIR)/TransportationPlannerConfig.h 
	$(CXX) -o $@ -c $(CXXFLAGS) $(TEST_SRC_DIR)/TPCommandLineTest.cpp

$(OBJ_DIR)/StandardDataSink.o: $(SRC_DIR)/StandardDataSink.cpp $(INC_DIR)/StandardDataSink.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(SRC_DIR)/StandardDataSink.cpp

$(OBJ_DIR)/StandardDataSource.o: $(SRC_DIR)/StandardDataSource.cpp $(INC_DIR)/StandardDataSource.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(SRC_DIR)/StandardDataSource.cpp

$(OBJ_DIR)/StandardErrorDataSink.o: $(SRC_DIR)/StandardErrorDataSink.cpp $(INC_DIR)/StandardErrorDataSink.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(SRC_DIR)/StandardErrorDataSink.cpp


# testtp
$(BIN_DIR)/testtp: $(OBJ_DIR)/FileDataSource.o $(OBJ_DIR)/FileDataFactory.o $(OBJ_DIR)/StandardDataSink.o $(OBJ_DIR)/StandardDataSource.o $(OBJ_DIR)/StandardErrorDataSink.o $(OBJ_DIR)/DijkstraTransportationPlanner.o $(OBJ_DIR)/CDijkstraPathRouter.o $(OBJ_DIR)/CSVOSMTransportationPlannerTest.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/XMLWriter.o $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/DSVWriter.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/FileDataSink.o $(OBJ_DIR)/StringUtils.o $(OBJ_DIR)/KMLWriter.o $(OBJ_DIR)/GeographicUtils.o
	$(CXX) -o $@ $^ $(LDFLAGS)
	@echo "success 11..."
	@echo ""

$(OBJ_DIR)/GeographicUtils.o: $(SRC_DIR)/GeographicUtils.cpp $(INC_DIR)/GeographicUtils.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(SRC_DIR)/GeographicUtils.cpp

$(OBJ_DIR)/DijkstraTransportationPlanner.o: $(SRC_DIR)/CDijkstraTransportationPlanner.cpp $(INC_DIR)/DijkstraTransportationPlanner.h $(INC_DIR)/TransportationPlanner.h $(INC_DIR)/GeographicUtils.h $(INC_DIR)/StandardDataSink.h $(INC_DIR)/StandardDataSource.h $(INC_DIR)/StandardErrorDataSink.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(SRC_DIR)/CDijkstraTransportationPlanner.cpp

$(OBJ_DIR)/CSVOSMTransportationPlannerTest.o: $(TEST_SRC_DIR)/CSVOSMTransportationPlannerTest.cpp $(INC_DIR)/DijkstraTransportationPlanner.h $(INC_DIR)/GeographicUtils.h $(INC_DIR)/StandardDataSink.h $(INC_DIR)/StandardDataSource.h $(INC_DIR)/StandardErrorDataSink.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(TEST_SRC_DIR)/CSVOSMTransportationPlannerTest.cpp


# testkml
$(BIN_DIR)/testkml: $(OBJ_DIR)/KMLWriter.o $(OBJ_DIR)/KMLTest.o $(OBJ_DIR)/XMLWriter.o $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringUtils.o
	$(CXX) -o $@ $^ $(LDFLAGS)
	@echo "success 12..."
	@echo ""

$(OBJ_DIR)/KMLWriter.o: $(SRC_DIR)/KMLWriter.cpp $(INC_DIR)/KMLWriter.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(SRC_DIR)/KMLWriter.cpp

$(OBJ_DIR)/KMLTest.o: $(TEST_SRC_DIR)/KMLTest.cpp $(INC_DIR)/KMLWriter.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(TEST_SRC_DIR)/KMLTest.cpp


transplanner: $(OBJ_DIR)/transplanner.o $(OBJ_DIR)/StringUtils.o  $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/OpenStreetMap.o
	$(CXX) -o $(BIN_DIR)/transplanner $(CXXFLAGS) $^ $(LDFLAGS)
	@echo "transplanner compiled successfully"
	@echo ""

# speedtest
speedtest: $(OBJ_DIR)/SpeedTest.o $(OBJ_DIR)/StringUtils.o  $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/OpenStreetMap.o
	$(CXX) -o $(BIN_DIR)/speedtest $(CXXFLAGS) $^ $(LDFLAGS)
	@echo "speedtest compiled successfully"
	@echo ""




$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/tests/%.o: $(SRC_DIR)/tests/%.cpp
	@mkdir -p $(OBJ_DIR)/tests
	$(CXX) $(CXXFLAGS) -c $< -o $@

testdpr: $(BIN_DIR)/testdpr
	@echo "Running testdpr..."
	@./$(BIN_DIR)/testdpr

testtp: $(BIN_DIR)/testtp
	@echo "Running testtp..."
	@./$(BIN_DIR)/testtp

testtpcl: $(BIN_DIR)/testtpcl
	@echo "Running testtpcl..."
	@./$(BIN_DIR)/testtpcl

testkml: $(BIN_DIR)/testkml
	@echo "Running testkml..."
	@./$(BIN_DIR)/testkml

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)

directories:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)