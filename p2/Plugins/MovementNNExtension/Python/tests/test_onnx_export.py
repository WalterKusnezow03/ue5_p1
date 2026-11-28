import contextlib
import io
import unittest
from unittest.mock import patch

import torch.nn as nn

from Base.Net import NetCheckpoint
from Base.Net.TensorInput.FTensor import FTensor


class OnnxExportTests(unittest.TestCase):
    def test_failed_export_does_not_load_model(self):
        net = nn.Linear(4, 4)
        tensor = FTensor(20, 1, 4)
        for export, tensors in (
            (NetCheckpoint.ExportNetSingleTensor, tensor),
            (NetCheckpoint.ExportNetMultiTensor, [tensor, tensor]),
        ):
            with self.subTest(export=export.__name__):
                output = io.StringIO()
                with patch.object(NetCheckpoint.torch.onnx.utils, "export", side_effect=RuntimeError("export failed")), \
                     patch.object(NetCheckpoint, "DebugOnnxExport") as debug, \
                     contextlib.redirect_stdout(output):
                    export(net, "test", tensors, "model.onnx")
                debug.assert_not_called()
                self.assertIn("export failed", output.getvalue())
                self.assertNotIn("FINISHED", output.getvalue())
                self.assertNotIn("FINSHED", output.getvalue())


if __name__ == "__main__":
    unittest.main()
