CC = gcc
CXX = g++

INC_DIR = ./include
SRC_DIR = ./src
OBJ_DIR = ./obj
BIN_DIR = ./bin
TEST_SRC_DIR = ./testsrc

CXXFLAGS = -std=c++17 -I $(INC_DIR) -Wall
LDFLAGS = -lgtest -lgtest_main -lpthread -lexpat

all: directories runtests 

test: runtests

runtests: $(BIN_DIR)/teststrutils $(BIN_DIR)/teststrdatasource $(BIN_DIR)/teststrdatasink $(BIN_DIR)/testdsv $(BIN_DIR)/testxml $(BIN_DIR)/testcsvbs $(BIN_DIR)/testosm
	@echo "Running all tests..."
	@./$(BIN_DIR)/teststrutils
	@./$(BIN_DIR)/teststrdatasource
	@./$(BIN_DIR)/teststrdatasink
	@./$(BIN_DIR)/testdsv
	@./$(BIN_DIR)/testxml
	@./$(BIN_DIR)/testcsvbs
	@./$(BIN_DIR)/testosm
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

# speedtest
# speedtest: $(OBJ_DIR)/SpeedTest.o $(OBJ_DIR)/StringUtils.o  $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/CSVBusSystem.o $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/OpenStreetMap.o $(OBJ_DIR)/OpenStreetMap.o
# 	$(CXX) -o $(BIN_DIR)/speedtest $(CXXFLAGS) $^ $(LDFLAGS)
# 	@echo "speedtest compiled successfully"
# 	@echo ""

# transplanner

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/tests/%.o: $(SRC_DIR)/tests/%.cpp
	@mkdir -p $(OBJ_DIR)/tests
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)

directories:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)