import os
import tempfile
import unittest

import customcsv


class TestCustomCSVReader(unittest.TestCase):
    def setUp(self):
        # Create a temporary CSV file for testing
        self.csv_data = "Name,Age,Location\nJohn,25,New York\nJane,30,San Francisco\n"
        self.temp_csv_file = tempfile.NamedTemporaryFile(mode="w+", delete=False)
        self.temp_csv_file.write(self.csv_data)
        self.temp_csv_file.close()

    def tearDown(self):
        # Clean up the temporary CSV file
        os.remove(self.temp_csv_file.name)

    def test_read_csv_file(self):
        # Test the read_csv_file function with the temporary CSV file
        result = customcsv.read_csv_file(self.temp_csv_file.name)

        # Expected result based on the CSV data
        expected_result = [
            ["Name", "Age", "Location"],
            ["John", "25", "New York"],
            ["Jane", "30", "San Francisco"],
        ]

        # Assert that the actual result matches the expected result
        self.assertListEqual(list(result), expected_result)


if __name__ == "__main__":
    unittest.main()

# python test.py
